//
//  TestColorVaryScene.h
//  Kaiju
//
//  Created by Jeremy Vineyard on 4/18/23.
//

#ifndef TestMeshPathScene_h
#define TestMeshPathScene_h

#include <PhaseJump/PhaseJump.h>

using namespace PJ;

class TestMeshPathScene : public Scene {
public:
    void LoadInto(World& world) {
        foo1(world);
        foo2(world);
    }

    void foo1(World& world) {
        auto pathNode = MAKE<WorldNode>();
        auto pathComponent = std::make_shared<CirclePathLayout2D>(500);
        pathNode->AddComponent(pathComponent);
        world.Add(pathNode);

        int count = 20;
        for (int i = 0; i < count; i++) {
            auto meshNode = MAKE<WorldNode>();
            auto meshRenderer = MAKE<MeshRenderer>();
            meshNode->AddComponent(meshRenderer);

            float progress = (float)i/(float)count;

            auto material = MAKE<RenderMaterial>();
            auto program = GLShaderProgram::registry["color.uniform"];
            if (program) {
                material->shaderProgram = program;
                material->uniformColors.Add(Color(progress, 1.0f - progress, 0, 1));

                EllipseRenderMeshBuilder builder(Angle::DegreesAngle(5.0f), Vector2(40, 20));
                auto renderMesh = builder.BuildRenderMesh();
                meshRenderer->material = material;
                meshRenderer->mesh = renderMesh;
            }
    //        window->World()->Add(meshNode);
            meshNode->transform->SetScale(Vector3(1.0f + progress, 1.0f + progress, 1));
    //        meshNode->transform->rotation.z = -(progress * 360.0f);
            pathNode->AddChild(meshNode);
        }

        pathComponent->ApplyLayout();
    }

    void foo2(World& world) {
        auto pathNode = MAKE<WorldNode>();
        auto pathComponent = std::make_shared<CirclePathLayout2D>(500);
        pathNode->AddComponent(pathComponent);
        world.Add(pathNode);
//        pathNode->transform->scale.x = 0.5f;
//        pathNode->transform->position.x = 300;
//        pathNode->transform->position.z = -0.5f;
//        pathNode->transform->rotation.z = -45;

        int count = 20;
        for (int i = 0; i < count; i++) {
            auto meshNode = MAKE<WorldNode>();
            auto meshRenderer = MAKE<MeshRenderer>();
            meshNode->AddComponent(meshRenderer);

            auto material = MAKE<RenderMaterial>();
            auto program = GLShaderProgram::registry["color.uniform"];
            if (program) {
                material->shaderProgram = program;
                material->uniformColors.Add(Color(1.0f, 0, 0, 1));

                EllipseRenderMeshBuilder builder(Angle::DegreesAngle(5.0f), Vector2(40, 20));
                auto renderMesh = builder.BuildRenderMesh();
                meshRenderer->material = material;
                meshRenderer->mesh = renderMesh;
            }
            pathNode->AddChild(meshNode);
        }

        pathComponent->orientToPath = false;
        pathComponent->ApplyLayout();
    }
};

#endif
