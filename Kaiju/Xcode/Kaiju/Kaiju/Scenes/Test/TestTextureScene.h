//
//  TestColorVaryScene.h
//  Kaiju
//
//  Created by Jeremy Vineyard on 4/18/23.
//

#ifndef TestTextureScene_h
#define TestTextureScene_h

#include <PhaseJump/PhaseJump.h>

using namespace PJ;

class TestTextureScene : public Scene {
public:
    void LoadInto(World& world) {
        auto meshNode = std::make_shared<WorldNode>();
        auto meshRenderer = std::make_shared<MeshRenderer>();
        meshNode->AddComponent(meshRenderer);

        auto material = std::make_shared<RenderMaterial>();
        auto program = GLShaderProgram::registry["textureUniform"];
        if (program) {
            material->shaderProgram = program;
            material->textures.Add(RenderTexture(1));

//            material->colors.Add(Color::white);
//            material->colors.Add(Color::red);
//            material->colors.Add(Color::white);
//            material->colors.Add(Color::red);

//            material->uniformColors.Add(Color::blue);

            material->features.Add(RenderFeatures::Blend);

            QuadRenderMeshBuilder builder(Vector2(400, 400));
            auto renderMesh = builder.BuildRenderMesh();
            meshRenderer->material = material;
            meshRenderer->mesh = renderMesh;
        }
        meshNode->transform->position.z = -0.1f;
        world.Add(meshNode);
    }
};

#endif
