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

class TestStateHandler : public SomeStateHandler<ButtonControl::StateType> {
    using StateType = ButtonControlStateType;

    void OnStateChange(StateType state) override {
        switch (state) {
            case StateType::Press:
                owner.lock()->transform->SetScale(Vector2::Uniform(3.0f));
                break;
            default:
                owner.lock()->transform->SetScale(Vector2::Uniform(1.0f));
                break;
        }
    }
};

class TestTextureScene : public Scene {
public:
    SP<GLTexture> texture;

    TestTextureScene(SP<GLTexture> texture) : texture(texture) {
    }

    void LoadInto(World& world) {
        if (nullptr == texture) { return; }

        auto meshNode = MAKE<WorldNode>();
//        auto renderer = MAKE<MeshRenderer>();
//        meshNode->AddComponent(renderer);

        auto renderer = MAKE<SpriteRenderer>(texture);
        meshNode->AddComponent(renderer);

//        auto material = MAKE<RenderMaterial>();
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
        meshNode->transform->SetWorldPosition(Vector3(200, 0, -0.2f));
//        meshNode->transform->scale.x = 10.0f;
//        meshNode->transform->scale.y = 10.0f;
//        renderer->flipX = true;
//        renderer->flipY = true;

//        meshNode->SetActive(false);

//        auto uiSystem = MAKE<UISystem>();
//        world.AddComponent(uiSystem);

        auto button = MAKE<ButtonControl>();
        auto ts = MAKE<TestStateHandler>();
        auto dragHandler = MAKE<DragHandler2D>();

        ComponentTool ct;
        auto collider = std::make_shared<SimplePolygonCollider2D>();
        ct.AddComponent(*meshNode, collider);
//        ct.AddComponent(*meshNode, button);
        ct.AddComponent(*meshNode, dragHandler);
        ct.AddComponent(*meshNode, ts);

        world.Add(meshNode);
    }
};

#endif
