#pragma once

#include <PhaseJump/PhaseJump.h>

using namespace PJ;

class TestAnimatedTexturesScene : public Scene {
public:
    using Base = Scene;
    using TextureList = VectorList<SP<SomeTexture>>;

    TextureList textures;

    TestAnimatedTexturesScene(TextureList const& textures) : textures(textures) {
    }

    void LoadInto(World& world) {
        if (textures.size() == 0) { return; }

        auto meshNode = MAKE<WorldNode>();
        auto renderer = MAKE<AnimatedSpriteRenderer>(textures);
        meshNode->AddComponent(renderer);

        auto material = renderer->material;

        auto program = GLShaderProgram::registry["texture.uniform"];
        if (program) {
            material->shaderProgram = program;
            material->features[RenderFeatures::Blend] = RenderFeatureStatus::Enable;
        }
        meshNode->transform->SetWorldPosition(Vector3(0, 0, -0.2f));

        world.Add(meshNode);
    }
};
