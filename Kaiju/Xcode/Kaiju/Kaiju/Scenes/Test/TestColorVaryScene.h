//
//  TestColorVaryScene.h
//  Kaiju
//
//  Created by Jeremy Vineyard on 4/18/23.
//

#ifndef TestColorVaryScene_h
#define TestColorVaryScene_h

#include <PhaseJump/PhaseJump.h>

using namespace PJ;

class TestColorVaryScene : public Scene {
public:
    void LoadInto(World& world) {
        auto meshNode = std::make_shared<WorldNode>();
        auto meshRenderer = std::make_shared<MeshRenderer>();
        meshNode->AddComponent(meshRenderer);

        auto material = std::make_shared<RenderMaterial>();
        auto program = GLShaderProgram::registry["colorVary"];
        if (program) {
            material->shaderProgram = program;
            material->colors.Add(Color::white);
            material->colors.Add(Color::black);
            material->colors.Add(Color::white);
            material->colors.Add(Color::black);

            QuadRenderMeshBuilder builder(Vector2(400, 400));
            auto renderMesh = builder.BuildRenderMesh();
            meshRenderer->material = material;
            meshRenderer->mesh = renderMesh;
        }
        world.Add(meshNode);
    }
};

#endif
