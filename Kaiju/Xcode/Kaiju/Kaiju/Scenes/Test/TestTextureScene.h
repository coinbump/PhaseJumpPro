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

        {
            auto sliceTexture = DCAST<GLTexture>(world.loadedResources->map["texture"]["slider-track"].resource);

            auto trackNode = MAKE<WorldNode>();
            std::array<Vector2Int, 2> slicePoints{Vector2Int(12, 12), Vector2Int(12, 12)};
            auto trackRenderer = MAKE<SlicedTextureRenderer>(sliceTexture, Vector2(300, 33), slicePoints);
            trackNode->AddComponent(trackRenderer);

            auto trackMaterial = trackRenderer->material;
            trackMaterial->shaderProgram = GLShaderProgram::registry["texture.uniform"];
            trackMaterial->features[RenderFeatures::Blend] = RenderFeatureStatus::Enable;

            auto animateHueEffect = MAKE<AnimateHueEffect>();
            animateHueEffect->SetIsOn(true);

            auto thumbTexture = DCAST<GLTexture>(world.loadedResources->map["texture"]["slider-thumb"].resource);
            auto thumbNode = MAKE<WorldNode>();
            auto thumbRenderer = MAKE<SpriteRenderer>(thumbTexture);
            thumbNode->AddComponent(thumbRenderer);

            auto thumbMaterial = thumbRenderer->material;
            thumbMaterial->shaderProgram = GLShaderProgram::registry["texture.uniform"];
            thumbMaterial->features[RenderFeatures::Blend] = RenderFeatureStatus::Enable;

            auto sliderControl = MAKE<SliderControl>();
            sliderControl->endCapSize = 10;
            trackNode->AddComponent(sliderControl);
            trackNode->AddChild(thumbNode);

            ComponentTool ct;
            auto collider = MAKE<SimplePolygonCollider2D>();
            ct.AddComponent(*thumbNode, collider);
            ct.AddComponent(*thumbNode, animateHueEffect);

            world.Add(trackNode);
        }

        {
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
            auto animateHueEffect = MAKE<AnimateHueEffect>();
            animateHueEffect->SetIsOn(true);

            ComponentTool ct;
            auto collider = MAKE<SimplePolygonCollider2D>();
            ct.AddComponent(*meshNode, collider);
            //        ct.AddComponent(*meshNode, button);
            ct.AddComponent(*meshNode, dragHandler);
            ct.AddComponent(*meshNode, ts);
//            ct.AddComponent(*meshNode, animateHueEffect);

            world.Add(meshNode);
        }
    }
};

#endif
