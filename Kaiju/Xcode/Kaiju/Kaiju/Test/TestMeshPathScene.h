#ifndef TestMeshPathScene_h
#define TestMeshPathScene_h

#include <PhaseJump/PhaseJump.h>

using namespace PJ;

class TestMeshPathScene : public Scene {
public:
    TestMeshPathScene() {
    }
    
    void LoadInto(WorldNode& root) {
        root.name = "TestMeshPathScene";

        auto camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());
        auto cameraNode = MAKE<WorldNode>("Camera");
        cameraNode->Add(camera);
        root.Add(cameraNode);

        foo1(root);
        foo2(root);
    }

    void foo1(WorldNode& root) {
        auto pathNode = MAKE<WorldNode>("Circle path");
        auto pathComponent = std::make_shared<CirclePathLayout2D>(500);
        pathNode->Add(pathComponent);
        root.Add(pathNode);

        int count = 20;
        for (int i = 0; i < count; i++) {
            auto meshNode = MAKE<WorldNode>("Mesh");
            auto meshRenderer = MAKE<MeshRenderer>();
            meshNode->Add(meshRenderer);

            float progress = (float)i/(float)count;

            auto material = MAKE<RenderMaterial>();
            auto program = SomeShaderProgram::registry["color.uniform"];
            if (program) {
                material->SetShaderProgram(program);
                material->AddUniformColor(Color(progress, 1.0f - progress, 0, 1));

                EllipseMeshBuilder builder(Angle::DegreesAngle(5.0f), Vector2(40, 20));
                auto renderMesh = builder.BuildMesh();
                meshRenderer->material = material;
                meshRenderer->mesh = renderMesh;
            }
    //        window->World()->Add(meshNode);
            meshNode->transform.SetScale(Vector3(1.0f + progress, 1.0f + progress, 1));
    //        meshNode->transform.rotation.z = -(progress * 360.0f);
            pathNode->Add(meshNode);
        }

        pathComponent->ApplyLayout();
    }

    void foo2(WorldNode& root) {
        auto pathNode = MAKE<WorldNode>("Circle path");
        auto pathComponent = std::make_shared<CirclePathLayout2D>(500);
        pathNode->Add(pathComponent);
        root.Add(pathNode);
//        pathNode->transform.scale.x = 0.5f;
//        pathNode->transform.position.x = 300;
//        pathNode->transform.position.z = -0.5f;
//        pathNode->transform.rotation.z = -45;

        int count = 20;
        for (int i = 0; i < count; i++) {
            auto meshNode = MAKE<WorldNode>("Mesh");
            auto meshRenderer = MAKE<MeshRenderer>();
            meshNode->Add(meshRenderer);

            auto material = MAKE<RenderMaterial>();
            auto program = SomeShaderProgram::registry["color.uniform"];
            if (program) {
                material->SetShaderProgram(program);
                material->AddUniformColor(Color(1.0f, 0, 0, 1));

                EllipseMeshBuilder builder(Angle::DegreesAngle(5.0f), Vector2(40, 20));
                auto renderMesh = builder.BuildMesh();
                meshRenderer->material = material;
                meshRenderer->mesh = renderMesh;
            }
            pathNode->Add(meshNode);
        }

        pathComponent->orientToPath = false;
        pathComponent->ApplyLayout();
    }
};

#endif
