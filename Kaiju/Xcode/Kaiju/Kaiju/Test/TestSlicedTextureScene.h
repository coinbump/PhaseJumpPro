#pragma once

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif

using namespace PJ;

/// Test scene for sliced SlicedTextureRenderer
class TestSlicedTextureScene : public Scene {
public:
    SP<GLTexture> texture;

    TestSlicedTextureScene() {
    }

    void LoadInto(WorldNode& root) {
        root.name = "TestSlicedTextureScene";

        World& world = *root.World();
        
        texture = DCAST<GLTexture>(world.FindTexture("example-button-normal"));
        GUARD(texture)

        auto camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());
        auto cameraNode = MAKE<WorldNode>("Camera");
        cameraNode->Add(camera);
        root.Add(cameraNode);

        for (int i = 0; i < 1; i++) {
            auto meshNode = MAKE<WorldNode>("Sliced texture");

            SlicedTextureRenderer::SlicePoints slicePoints = {{25, 25}, {25, 25}};
            auto renderer = MAKE<SlicedTextureRenderer>(texture, Vector2(300, 300), slicePoints);
            meshNode->Add(renderer);
            meshNode->transform.SetLocalPosition(Vector3(i * 10, i * 10, i * .001f));

            auto material = renderer->model.material;

            root.Add(meshNode);
        }
    }
};
