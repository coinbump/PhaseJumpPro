#pragma once

#include <PhaseJump/PhaseJump.h>

//namespace PJ {
//    class Updater : public WorldComponent<> {
//    }
//}

using namespace PJ;


class TestGradientsScene : public Scene {
public:
    TestGradientsScene() {
    }

    void LoadInto(WorldNode& root) {
        root.name = "TestGradientsScene";

        auto camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());
        auto cameraNode = MAKE<WorldNode>("Camera");
        cameraNode->Add(camera);
        root.Add(cameraNode);

        for (int i = 0; i < 1; i++) {
            auto gradientNode = MAKE<WorldNode>("Gradient");
//            auto gradientRenderer = MAKE<SimpleGradientRenderer>(Color::white, Color::black.WithAlpha(1), Vector2(400, 400));
            auto gradientRenderer = MAKE<SimpleGradientRenderer>(Color::black, Color::black.WithAlpha(1), Vector2(400, 400));

            // TODO: ArcMeshBuilder, CenterPolyMeshBuilder, EllipseMeshBuilder, QuadMeshBuilder, QuadFrameMeshBuilder, CenterPolyFrameMeshBuilder

            gradientRenderer->model.SetMeshBuilderFunc([](RendererModel const& model) {
                QuadFrameMeshBuilder meshBuilder(model.WorldSize(), Vector2(20, 20));
                return meshBuilder.BuildMesh();
            });

            gradientRenderer->model.SetMeshBuilderFunc([](RendererModel const& model) {
                EllipseMeshBuilder meshBuilder(model.WorldSize());
                return meshBuilder.BuildMesh();
            });

            gradientRenderer->model.SetMeshBuilderFunc([](RendererModel const& model) {
                CenterPolyMeshBuilder meshBuilder(model.WorldSize(), CenterPolyShape::circle);
                return meshBuilder.BuildMesh();
            });

            gradientRenderer->model.SetMeshBuilderFunc([](RendererModel const& model) {
                QuadMeshBuilder meshBuilder(model.WorldSize());
                return meshBuilder.BuildMesh();
            });

            gradientRenderer->model.SetMeshBuilderFunc([](RendererModel const& model) {
                CenterPolyFrameMeshBuilder meshBuilder(model.WorldSize(), 10, CenterPolyShape::square);
                return meshBuilder.BuildMesh();
            });

            gradientRenderer->model.SetMeshBuilderFunc([](RendererModel const& model) {
                Polygon poly;
                poly.Add(Vector3(-100, 100, 0));
                poly.Add(Vector3(100, 100, 0));
                poly.Add(Vector3(0, -100, 0));
                PolyFrameMeshBuilder meshBuilder(poly, 10);
                return meshBuilder.BuildMesh();
            });


            gradientNode->Add(gradientRenderer);

            auto steering = MAKE<VelocityKSteering>(Vector3(20, 20, 0));
            gradientNode->Add(steering);

            //gradientNode->Destroy(5);

            auto updater = MAKE<WorldComponent<MultiFuncWorldComponentCore>>();
            updater->core.OnUpdateFuncs().Add([=](auto& component, TimeSlice time) {
                auto worldSize = gradientRenderer->WorldSize();
                GUARD(worldSize)

                worldSize->x += 10 * time.delta;

                /// Use x twice
//                gradientRenderer->SetWorldSize(Vector2(worldSize->x, worldSize->x));

//                gradientNode->transform.SetRotation(Angle::DegreesAngle(-gradientNode->transform.Rotation().z + 45.0f * time.delta));
            });

            gradientNode->Add(updater);

            root.Add(gradientNode);
//            gradientNode->SetLocalScale(Vector3::one * ((float)i / 30.0f) * 3.0f);
        }
    }
};
