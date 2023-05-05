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
        auto meshNode = MAKE<WorldNode>();
        auto meshRenderer = MAKE<MeshRenderer>();
        meshNode->AddComponent(meshRenderer);

        auto material = MAKE<RenderMaterial>();
        auto program = GLShaderProgram::registry["color.vary"];
        if (program) {
            material->shaderProgram = program;
            material->colors.Add(Color::white);
            material->colors.Add(Color::black.WithAlpha(1));
            material->colors.Add(Color::white);
            material->colors.Add(Color::black.WithAlpha(1));
            material->features[RenderFeatures::Blend] = RenderFeatureStatus::Enable;

            QuadRenderMeshBuilder builder(Vector2(400, 400));
            auto renderMesh = builder.BuildRenderMesh();
            meshRenderer->material = material;
            meshRenderer->mesh = renderMesh;
        }
        world.Add(meshNode);
    }
};

#endif
