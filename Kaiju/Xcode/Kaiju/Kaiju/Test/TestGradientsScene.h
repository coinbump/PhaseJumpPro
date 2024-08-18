#pragma once

#include <PhaseJump/PhaseJump.h>

//namespace PJ {
//    class Updater : public WorldComponent<> {
//    }
//}

using namespace PJ;


class TestGradientsScene : public Scene {
public:
    void LoadInto(WorldNode& root) {
        auto camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());
        auto cameraNode = MAKE<WorldNode>();
        cameraNode->Add(camera);
        root.Add(cameraNode);

        for (int i = 0; i < 30; i++) {
            auto gradientNode = MAKE<WorldNode>();
            auto gradientRenderer = MAKE<SimpleGradientRenderer>(Color::white, Color::black.WithAlpha(1), Vector2(400, 400));
            gradientRenderer->model.SetMeshBuilderFunc([](MeshRendererModel const& model) {
                // FUTURE: support colors builder for different meshes (vertices are different, so colors are wrong)
                QuadFrameMeshBuilder meshBuilder(model.WorldSize(), Vector2(20, 20));
                return meshBuilder.BuildMesh();

                //            EllipseMeshBuilder meshBuilder(Angle::DegreesAngle(10), worldSize);
                //            return meshBuilder.BuildMesh();
            });
            gradientNode->Add(gradientRenderer);

            auto steering = MAKE<VelocityKSteering>(Vector3(20, 20, 0));
            gradientNode->Add(steering);

            //gradientNode->Destroy(5);

            auto updater = MAKE<WorldComponent<ComposeWorldComponentCore>>();
            updater->core.updateFuncs.Add([=](auto& component, TimeSlice time) {
                auto worldSize = gradientRenderer->WorldSize2D();
                worldSize.x += 10 * time.delta;

                /// Use x twice
                gradientRenderer->SetWorldSize2D(Vector2(worldSize.x, worldSize.x));

                gradientNode->transform->SetRotation(Angle::DegreesAngle(-gradientNode->transform->Rotation().z + 45.0f * time.delta));
            });

            gradientNode->Add(updater);

            root.Add(gradientNode);
            gradientNode->SetLocalScale(Vector3::one * ((float)i / 30.0f) * 3.0f);
        }
    }
};
