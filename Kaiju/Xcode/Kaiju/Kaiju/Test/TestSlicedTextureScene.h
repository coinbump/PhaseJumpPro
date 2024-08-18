#pragma once

#include <PhaseJump/PhaseJump.h>

using namespace PJ;

/// Test scene for sliced SlicedTextureRenderer
class TestSlicedTextureScene : public Scene {
public:
    SP<GLTexture> texture;

    TestSlicedTextureScene() {
    }

    void LoadInto(WorldNode& root) {
        World& world = *root.world;
        
        texture = DCAST<GLTexture>(world.loadedResources->map["texture"]["example-button-normal"].resource);
        GUARD(texture)

        auto camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());
        auto cameraNode = MAKE<WorldNode>();
        cameraNode->Add(camera);
        root.Add(cameraNode);

        auto meshNode = MAKE<WorldNode>();

        std::array<Vector2Int, 2> slicePoints = {Vector2Int(25, 25), Vector2Int(25, 25)};
        auto renderer = MAKE<SlicedTextureRenderer>(texture, Vector2(300, 300), slicePoints);
        meshNode->Add(renderer);

        auto material = renderer->material;
        
        auto program = GLShaderProgram::registry["texture.uniform"];
        if (program) {
            material->SetShaderProgram(program);
            material->EnableFeature(RenderFeature::Blend, true);
        }

        root.Add(meshNode);
    }
};
