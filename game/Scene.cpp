#include "Scene.hpp"
#include "MyProfiler.hpp"
#include "BiomeZoom.hpp"
#include "BiomeConst.hpp"
#include "BiomeReplace.hpp"
#include "BiomeIsland.hpp"
#include "BiomeOutline.hpp"

Scene::Scene() {
    //Setup gl stuff
    srand(0);
    Mouse::setRelativeMode(true); Mouse::setGrab(true);
    GL_ASSERT(glClearColor(0, 0, 0, 1));
    GL_ASSERT(glEnable(GL_DEPTH_TEST));
    GL_ASSERT(glEnable(GL_BLEND));
    GL_ASSERT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_ASSERT(glDepthFunc(GL_LEQUAL));
    GL_ASSERT(glEnable(GL_CULL_FACE)); //enable backface culling
    GL_ASSERT(glCullFace(GL_BACK));
    GL_ASSERT(glEnable(GL_FRAMEBUFFER_SRGB));

    //instantiate profiler
    MyProfiler* p = new MyProfiler();
    p->addTo(this);

    //build quad, program & texture
    std::vector<Vertex::Attribute> elems = {
        Vertex::Attribute("a_position", Vertex::Attribute::Float, 3),
        Vertex::Attribute("a_texcoord", Vertex::Attribute::Float, 2)
    };
    struct Vert {
            vec3f p;
            vec2f t;
    };
    std::vector<Vert> data = {
        {vec3f( 1, -1, 0), vec2f(1, 0)},
        {vec3f( 1,  1, 0), vec2f(1)},
        {vec3f(-1,  1, 0), vec2f(0, 1)},
        {vec3f(-1, -1, 0), vec2f(0)},
    };
    std::vector<unsigned int> indexes = {
        0, 1, 2, 3, 0, 2
    };
    quad = MeshIndexed(Vertex::Format(elems));
    quad.setIndexData(&indexes[0], indexes.size());
    quad.setVertexData(&data[0], data.size());
    program = ShaderProgram(Storage::openAsset("shaders/tex.vert"),Storage::openAsset("shaders/tex.frag"));
    tex = Texture2D(vec2ui(Window::getInstance()->getSize().x/4,Window::getInstance()->getSize().y/4), TextureFormat::SRGBA8);
    tex.setFilter(GL_NEAREST, GL_NEAREST);

    // Build func
    generator.seed(13213);
    func = new BiomeConst(&generator, 0);
    func = new BiomeReplace(&generator, func, BiomeSet({OCEAN}), PLAINS, 10, false);

    func = new BiomeZoom(&generator, func, true);
    func = new BiomeIsland(&generator, func);
    func = new BiomeZoom(&generator, func, false);
    func = new BiomeIsland(&generator, func);

    func = new BiomeReplace(&generator, func, BiomeSet({PLAINS}), ICEPLAINS, 5, false);

    func = new BiomeZoom(&generator, func, false);
    func = new BiomeIsland(&generator, func);
    func = new BiomeZoom(&generator, func, false);
    func = new BiomeIsland(&generator, func);

    func = new BiomeReplace(&generator, func, BiomeSet({OCEAN}), MUSHROOMISLAND, 100, true);

    BiomeFunction* river = func;
    //do river..

    //Non-icy
    func = new BiomeReplace(
        &generator,
        func,
        BiomeSet({1}),
        {
            {PLAINS, 1},
            {DESERT, 1},
            {EXTREMEHILLS, 1},
            {FOREST, 1},
            {TAIGA, 1},
            {SWAMPLAND, 1},
            {JUNGLE, 1},
        }
    );

    //Icy
    func = new BiomeReplace(&generator, func, BiomeSet({ICEPLAINS}), TAIGA, 4);

    func = new BiomeZoom(&generator, func, false);
    func = new BiomeZoom(&generator, func, false);

    //Hills
    func = new BiomeReplace(&generator, func, BiomeSet({DESERT}), DESERTHILLS, 3, true);
    func = new BiomeReplace(&generator, func, BiomeSet({FOREST}), FORESTHILLS, 3, true);
    func = new BiomeReplace(&generator, func, BiomeSet({TAIGA}), TAIGAHILLS, 3, true);
    func = new BiomeReplace(&generator, func, BiomeSet({PLAINS}), FOREST, 3, true);
    func = new BiomeReplace(&generator, func, BiomeSet({ICEPLAINS}), ICEMOUNTAINS, 3, true);
    func = new BiomeReplace(&generator, func, BiomeSet({JUNGLE}), JUNGLEHILLS, 3, true);
    func = new BiomeZoom(&generator, func, false);
    func = new BiomeIsland(&generator, func);
    func = new BiomeZoom(&generator, func, false);

    // Shores
    func = new BiomeOutline(&generator, func, BiomeSet({OCEAN}), BiomeSet({EXTREMEHILLS, SWAMPLAND, MUSHROOMISLAND}, true), BEACH, true);
    func = new BiomeOutline(&generator, func, BiomeSet({EXTREMEHILLS}), BiomeSet({}, true), EXTREMEHILLSEDGE, false);
    func = new BiomeOutline(&generator, func, BiomeSet({MUSHROOMISLAND}), BiomeSet({}, true), MUSHROOMISLANDSHORE, false);

    // Add ponds
    func = new BiomeReplace(&generator, func, BiomeSet({JUNGLE, JUNGLEHILLS}), RIVER, 8, false);
    func = new BiomeReplace(&generator, func, BiomeSet({SWAMPLAND}), RIVER, 6, false);

    // Last zooms
    func = new BiomeZoom(&generator, func, false);
    func = new BiomeZoom(&generator, func, false);

    genTexData();
}

void Scene::update(float deltaTime) {
    (void) deltaTime;
    if(Keyboard::justPressed(Keyboard::Escape) || Window::getInstance()->isClosing())
        getGame()->isRunning = false;
    MyProfiler* p = getGame()->getFirstObjectOfType<MyProfiler>();
    bool redraw = false;
    if(Mouse::pressed(Mouse::Left)) {
        if(!p->isShown()) {
            offset.x -= Mouse::movement().x*0.1;
            offset.y += Mouse::movement().y*0.1;
        }
        redraw = true;
    }
    if(redraw) genTexData();
}

void Scene::draw() const {
    GL_ASSERT(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
    program.uniform("tex")->set(tex);
    quad.draw(program);
}

float denorm(float f) {
    return f;
    float M = 0.5f;
    float SD = 0.1f;
    float z = (M-f)/SD;
    if(z < -7) return 0.0f;
    if(z > 7) return 1.0f;
    bool flag = z < 0.0f;
    z = glm::abs(z);
    float b = 0.0f;
    float s = glm::sqrt(2.0f)/3.0f*z;
    float HH = 0.5f;
    for(int i = 0; i < 12; ++i) {
        float a = glm::exp(-HH*HH/9)*glm::sin(HH*s)/HH;
        b += a;
        HH += 1.0f;
    }
    float p = 0.5f-b/glm::pi<float>();
    if(!flag) p = 1.0f-p;
    return p;
}

void Scene::genTexData() {
    vec2ui size = tex.getSize();
    static vec4uc* pixels = new vec4uc[size.x*size.y];
    vec3uc colors[23] = {
        {0, 0, 112},     // 0 Ocean
        {141, 179, 96},  // 1 Plains
        {250, 148, 24},  // 2 Desert
        {96, 96, 96},    // 3 Extreme Hills
        {5, 102, 33},    // 4 Forest
        {11, 102, 89},   // 5 Taiga
        {7, 249, 178},   // 6 Swampland
        {0, 0, 255},     // 7 River
        {255, 0, 0},     // 8 Hell
        {128, 128, 255}, // 9 Sky
        {128, 128, 255}, // 10 FrozenOcean
        {160, 160, 255}, // 11 FrozenRiver
        {255, 255, 255}, // 12 IcePlains
        {160, 160, 160}, // 13 IceMountains
        {255, 0, 255},   // 14 MushroomIsland
        {160, 0, 255},   // 15 MushroomIslandShore
        {250, 222, 85},  // 16 Beach
        {210, 95, 18},   // 17 DesertHills
        {34, 85, 28},    // 18 ForestHills
        {22, 57, 51},    // 19 TaigaHills
        {114, 120, 154}, // 20 Extreme Hills Edge
        {83, 123, 9},    // 21 Jungle
        {44, 66, 5},     // 22 JungleHills
    };
    std::vector<int> data = func->getBiomeData(offset.x, offset.y, size.x, size.y);
    for(unsigned int i = 0; i < size.x; ++i)
        for(unsigned int j = 0; j < size.y; ++j)
            pixels[i*size.y + j] = vec4uc(colors[data[i*size.y + j]], 255);
    tex.setData(&pixels[0]);
}
