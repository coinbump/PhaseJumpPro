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
    std::shared_ptr<GLTexture> texture;

    TestTextureScene(std::shared_ptr<GLTexture> texture) : texture(texture) {
    }

    void LoadInto(World& world) {
        if (nullptr == texture) { return; }

        auto meshNode = std::make_shared<WorldNode>();
//        auto renderer = std::make_shared<MeshRenderer>();
//        meshNode->AddComponent(renderer);

        auto renderer = std::make_shared<SpriteRenderer>((RenderTexture)*texture);
        meshNode->AddComponent(renderer);

//        auto material = std::make_shared<RenderMaterial>();
        auto material = renderer->material;
        
        auto program = GLShaderProgram::registry["texture.uniform"];//texture.interp.uniform"]; //"texture.uniform"
        if (program) {
            material->shaderProgram = program;
//            material->textures.Add(RenderTexture(texture->glId));

//            material->colors.Add(Color::white);
//            material->colors.Add(Color::red);
//            material->colors.Add(Color::white);
//            material->colors.Add(Color::red);

//            material->uniformColors.Add(Color::blue);
//            material->uniformFloats.Add(0);//0.5f);

            material->features[RenderFeatures::Blend] = RenderFeatureStatus::Enable;

//            QuadRenderMeshBuilder builder(Vector2(400, 400));
//            auto renderMesh = builder.BuildRenderMesh();
//            meshRenderer->material = material;
//            meshRenderer->mesh = renderMesh;
        }
        meshNode->transform->position.z = -0.1f;
        meshNode->transform->scale.x = 10.0f;
        meshNode->transform->scale.y = 10.0f;
        renderer->flipX = true;
//        renderer->flipY = true;

//        meshNode->SetActive(false);

        world.Add(meshNode);
    }
};

#endif
