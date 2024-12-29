#pragma once

#include "SDKIncludes.h"

using namespace PJ;

/// Test scene for sliced Slice9TextureRenderer
class TestSlicedTextureScene : public Scene {
public:
    SP<GLTexture> texture;

    TestSlicedTextureScene() {}

    void LoadInto(WorldNode& root) override {
        root.SetName("TestSlicedTextureScene");

        World& world = *root.World();

        texture = DCAST<GLTexture>(world.resources.FindTexture("example-button-normal"));
        GUARD(texture)

        auto camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());
        auto cameraNode = MAKE<WorldNode>("Camera");
        cameraNode->Add(camera);
        root.Add(cameraNode);

        for (int i = 0; i < 1; i++) {
            auto meshNode = MAKE<WorldNode>("Slice 9");

            Slice9TextureRenderer::SliceModel slicePoints = { { 25, 25 }, { 25, 25 } };
            auto renderer = MAKE<Slice9TextureRenderer>(texture, Vector2(300, 300), slicePoints);
            meshNode->Add(renderer);

            auto material = renderer->model.material;

            root.Add(meshNode);
        }

        {
            texture = DCAST<GLTexture>(world.resources.FindTexture("slider-track"));
            GUARD(texture)

            auto meshNode = MAKE<WorldNode>("Slice 3- X");

            auto renderer =
                MAKE<Slice3TextureRenderer>(Slice3TextureRenderer::Config{ .texture = texture,
                                                                           .axis = Axis2D::X,
                                                                           .axisLength = 300,
                                                                           .startInset = 25,
                                                                           .endInset = 25 });
            meshNode->Add(renderer);
            meshNode->transform.SetLocalPosition(Vector3(0, 200, 0));

            auto material = renderer->model.material;

            root.Add(meshNode);
        }

        {
            texture = DCAST<GLTexture>(world.resources.FindTexture("slider-track-v"));
            GUARD(texture)

            auto meshNode = MAKE<WorldNode>("Slice 3- Y");

            auto renderer =
                MAKE<Slice3TextureRenderer>(Slice3TextureRenderer::Config{ .texture = texture,
                                                                           .axis = Axis2D::Y,
                                                                           .axisLength = 300,
                                                                           .startInset = 25,
                                                                           .endInset = 25 });
            meshNode->Add(renderer);
            meshNode->transform.SetLocalPosition(Vector3(200, 0, 0));

            auto material = renderer->model.material;

            root.Add(meshNode);
        }

        {
            auto meshNode = MAKE<WorldNode>("Immediate");

            auto renderer = MAKE<ImRenderer>(ImRenderer::Config{ .worldSize = { 400, 400 } });
            meshNode->Add(renderer);
            meshNode->transform.SetLocalPosition(Vector3(200, 200, 0));

            renderer->AddSignalHandler({ .id = SignalId::RenderPrepare,
                                         .func = [](auto& renderer, auto& signal) {
                                             ImRenderer& im =
                                                 *(static_cast<ImRenderer*>(&renderer));

                                             im.SetColor(Color::yellow);

                                             im.FillRect({ .size = { 400, 20 } })
                                                 .FillRect({ .size = { 20, 400 } }, Color::red)
                                                 .FillRect({ .size = { 20, 20 } }, Color::blue);
                                         } });

            root.Add(meshNode);
        }
    }
};
