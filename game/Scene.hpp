#ifndef SCENE_HPP
#define SCENE_HPP
#include "commons.hpp"
#include "Noise.hpp"

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
        Noise n[2];
};

#endif // SCENE_HPP
