#pragma once

#include "SDKIncludes.h"

// namespace PJ {
//     class Updater : public WorldComponent<> {
//     }
// }

using namespace PJ;

class TestGradientsScene : public Scene {
public:
    TestGradientsScene() {}

    void LoadInto(WorldNode& root) override {
        root.name = "TestGradientsScene";

        auto camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());
        auto cameraNode = MAKE<WorldNode>("Camera");
        cameraNode->Add(camera);
        root.Add(cameraNode);

        for (int i = 0; i < 1; i++) {
            auto gradientNode = MAKE<WorldNode>("Gradient");
            auto gradientRenderer = MAKE<SimpleGradientRenderer>(
                Color::white, Color::black.WithAlpha(1), Vector2(400, 400)

            );
            //            auto gradientRenderer = MAKE<SimpleGradientRenderer>(Color::black,
            //            Color::black.WithAlpha(1), Vector2(400, 400));

            // TODO: ArcMeshBuilder, CenterPolyMeshBuilder, EllipseMeshBuilder, QuadMeshBuilder,
            // QuadFrameMeshBuilder, CenterPolyFrameMeshBuilder

            auto& ac = gradientNode->AddComponent<SimpleAnimationController>();

            auto quadFrameRenderer = MAKE<SimpleGradientRenderer>(
                Color::white, Color::black.WithAlpha(1), Vector2(400, 400)
            );
            quadFrameRenderer->model.SetBuildMeshFunc([](RendererModel const& model) {
                QuadFrameMeshBuilder meshBuilder(model.WorldSize(), Vector2(20, 20));
                return meshBuilder.BuildMesh();
            });

            ac.Add("0", [](auto& controller) {
                auto quadRenderer = MAKE<SimpleGradientRenderer>(
                    Color::white, Color::black.WithAlpha(1), Vector2(400, 400)
                );
                quadRenderer->model.SetBuildMeshFunc([](RendererModel const& model) {
                    QuadMeshBuilder meshBuilder(model.WorldSize());
                    return meshBuilder.BuildMesh();
                });

                auto renderer = MAKE<ColorRenderer>(ColorRenderer::Config{
                    .color = Color::black, .worldSize = { 400, 400 } });
                renderer->model.SetBuildMeshFunc([](RendererModel const& model) {
                    RoundCornersMeshBuilder meshBuilder(
                        { .worldSize = model.WorldSize(), .corners = RoundCorners::WithRadius(40) }
                    );
                    return meshBuilder.BuildMesh();
                });

                return renderer; // quadRenderer;
            });
            ac.Add("1", [](auto& controller) {
                auto renderer = MAKE<SimpleGradientRenderer>(
                    Color::white, Color::black.WithAlpha(1), Vector2(400, 400)
                );
                renderer->model.SetBuildMeshFunc([](RendererModel const& model) {
                    QuadFrameMeshBuilder meshBuilder(model.WorldSize(), Vector2(20, 20));
                    return meshBuilder.BuildMesh();
                });
                return renderer;
            });
            ac.Add("2", [](auto& controller) {
                auto renderer = MAKE<ColorRenderer>(ColorRenderer::Config{
                    .color = Color::black, .worldSize = { 400, 400 } });
                renderer->model.SetBuildMeshFunc([](RendererModel const& model) {
                    Polygon poly;
                    poly.Add({ -100, 100 });
                    poly.Add({ 100, 200 });
                    poly.Add({ 50, 300 });
                    poly.Add({ 0, -100 });
                    poly.Add({ -50, -75 });
                    poly.Add({ -40, -25 });
                    PolyFrameMeshBuilder meshBuilder({ .poly = poly,
                                                       .strokeWidth = 10,
                                                       .startCap = PathCap::Round,
                                                       .endCap = PathCap::Round,
                                                       .corner = PathCorner::Round,
                                                       .polyClose = PolyClose::Open });
                    return meshBuilder.BuildMesh();
                });
                return renderer;
            });

            ac.states.AddTransition("0", "next", "1");
            ac.states.AddTransition("1", "next", "2");
            ac.states.AddTransition("2", "next", "0");
            ac.states.SetState("0");

            ac.signalFuncs[SignalId::KeyDown] = [](auto& component, auto& signal) {
                auto& animationController = *(static_cast<SimpleAnimationController*>(&component));
                animationController.states.Input("next");
            };

            //
            //            gradientRenderer->model.SetBuildMeshFunc([](RendererModel const& model) {
            //                EllipseMeshBuilder meshBuilder(model.WorldSize());
            //                return meshBuilder.BuildMesh();
            //            });
            //
            //            gradientRenderer->model.SetBuildMeshFunc([](RendererModel const& model) {
            //                CenterPolyMeshBuilder meshBuilder(model.WorldSize(),
            //                CenterPolyShape::circle); return meshBuilder.BuildMesh();
            //            });
            //
            //            gradientRenderer->model.SetBuildMeshFunc([](RendererModel const& model) {
            //                QuadMeshBuilder meshBuilder(model.WorldSize());
            //                return meshBuilder.BuildMesh();
            //            });
            //
            //            gradientRenderer->model.SetBuildMeshFunc([](RendererModel const& model) {
            //                CenterPolyFrameMeshBuilder meshBuilder(model.WorldSize(), 10,
            //                CenterPolyShape::square); return meshBuilder.BuildMesh();
            //            });
            //

            //            gradientRenderer->model.SetBuildMeshFunc([](RendererModel const& model) {
            //                GridMeshBuilder meshBuilder(model.WorldSize(), Vector2Int(3, 3), 10);
            //                return meshBuilder.BuildMesh();
            //            });

            gradientNode->Add(gradientRenderer);

            auto steering = MAKE<VelocityKSteering>(Vector3(20, 20, 0));
            gradientNode->Add(steering);

            // gradientNode->Destroy(5);

            root.Add(gradientNode);
            //            gradientNode->SetLocalScale(Vector3::one * ((float)i / 30.0f) * 3.0f);
        }
    }

    // MARK: SomeWorldComponent

    String TypeName() const override {
        return "TestGradientScene";
    }
};
