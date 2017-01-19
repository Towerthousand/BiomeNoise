#ifndef SCENE_HPP
#define SCENE_HPP
#include "commons.hpp"
#include "Noise.hpp"

class BiomeFunction;
class Scene final : public GameObject {
    public:
        Scene();

    private:
        void update(float deltaTime) override;
        void draw() const override;
        void genTexData();

        vec2i offset = vec2i(0);

        MeshIndexed quad;
        ShaderProgram program;
        Texture2D tex;
        BiomeFunction* func = nullptr;
        std::mt19937 generator;

    enum Biome {
        OCEAN = 0,
        PLAINS,
        DESERT,
        EXTREMEHILLS,
        FOREST,
        TAIGA,
        SWAMPLAND,
        RIVER,
        HELL,
        SKY,
        FROZENOCEAN,
        FROZENRIVER,
        ICEPLAINS,
        ICEMOUNTAINS,
        MUSHROOMISLAND,
        MUSHROOMISLANDSHORE,
        BEACH,
        DESERTHILLS,
        FORESTHILLS,
        TAIGAHILLS,
        EXTREMEHILLSEDGE,
        JUNGLE,
        JUNGLEHILLS
    };
};

#endif // SCENE_HPP
