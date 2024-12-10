#pragma once

#include "SDKIncludes.h"

using namespace PJ;

class DestroyOnTap : public WorldComponent<> {};

class TestEmittersScene : public Scene {
public:
    using Base = Scene;
    using This = TestEmittersScene;

    SP<RenderMaterial> heartMaterial;

    TestEmittersScene() {
        PlanUIFunc planUIFunc = [](auto& component, String context, UIPlanner& planner) {
            auto concrete = static_cast<This*>(&component);

            planner.InputFloat(
                "Emit delta", { [=]() { return concrete->EmitDelta(); },
                                [=](auto& value) { concrete->SetEmitDelta(value); } }
            );
        };
        Override(planUIFuncs[UIContextId::Editor], planUIFunc);
    }

    float EmitDelta() {
        GUARDR(owner, 0)

        VectorList<Emitter*> emitters;
        owner->CollectChildTypeComponents<Emitter>(emitters);
        if (!IsEmpty(emitters)) {
            return static_cast<TimerDriver*>(emitters[0]->driver.get())->Duration();
        }
        return 0;
    }

    void SetEmitDelta(float value) {
        VectorList<Emitter*> emitters;
        owner->CollectChildTypeComponents<Emitter>(emitters);
        if (!IsEmpty(emitters)) {
            static_cast<TimerDriver*>(emitters[0]->driver.get())->SetDuration(value);
        }
    }

    void LoadInto(WorldNode& root) override {
        root.name = "TestEmittersScene";

        auto camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());
        auto cameraNode = MAKE<WorldNode>("Camera");
        cameraNode->Add(camera);
        root.Add(cameraNode);

        // #define ADD_RAYCASTER

#ifdef ADD_RAYCASTER
        auto raycaster = MAKE<SimpleRaycaster2D>();
        cameraNode->Add(raycaster);
        root.Add(cameraNode);
#endif

        World& world = *root.World();

        auto texture = DCAST<GLTexture>(world.FindTexture("heart-full"));
        GUARD(texture)

        heartMaterial =
            MAKE<RenderMaterial>(RenderMaterial::Config{ .shaderId = "texture.uniform" });
        heartMaterial->Add(texture);

        auto font = DCAST<Font>(world.FindFontWithSize(32));

        auto node = MAKE<WorldNode>("Emitter");
        Emitter::SpawnFunc spawnFunc = [this, font](Emitter& emitter, EmitModel emit) {
            auto node = MAKE<WorldNode>("Heart");

            // #define ACID_TEST

            // Used to test batching even if material isn't shared
#ifdef ACID_TEST
            auto texture = DCAST<GLTexture>(owner->World()->Texture("heart-full"));
            auto heartMaterial = MAKE<RenderMaterial>();
            heartMaterial->SetShaderProgram(SomeShaderProgram::registry["texture.uniform"]);
            heartMaterial->EnableFeature(RenderFeature::Blend, false);
            heartMaterial->Add(texture);
#endif
            auto spriteRenderer = MAKE<SpriteRenderer>(heartMaterial);
            node->Add(spriteRenderer);

            auto countString = MakeString(emitter.owner->ChildCount());

            WorldNode& textNode = node->And("Text " + countString);
            textNode
                .With<TextRenderer>(TextRenderer::Config{
                    .font = font, .text = countString, .worldSize = { 400, 400 } })
                .SetLineAlignFunc(AlignFuncs::center)
                .SetTextAlignFunc(AlignFuncs::center)
                .SetTextColor(Color::white)
                .SetLineClip(LineClip::None)
                .SizeToFit({ 400, 400 });

            // #define DESTROY_ON_TAP

            auto angle = emit.tags.SafeValue<Angle>(EmitModelTag::Angle);
            Vec2 angleVector = (Vector2)angle;
            Vec3 velocity = angleVector * 30;

            auto steering = MAKE<VelocityKSteering>(velocity);
            node->Add(steering);
            auto lp = node->transform.LocalPosition();
            lp.z = node->ChildCount() * -.001f;
            node->transform.SetLocalPosition(lp);

            return Emitter::SpawnList{ node };
        };
        auto emitter = MAKE<Emitter>(spawnFunc, .1f);

        // #define MAX_ALIVE

#ifdef MAX_ALIVE
        emitter->maxAlive = 5;
#endif

        emitter->emitFunc =
            EmitFuncs::MakeSpread2D(4, Angle::DegreesAngle(20.0f), Angle::DegreesAngle(3.0f));
        emitter->positionFunc = [](auto& emitter) {
            return CirclePositioner2D::MakeFunc(500.0f)(*emitter.random);
        };

        node->Add(emitter);
        root.Add(node);
    }

    // MARK: SomeWorldComponent

    String TypeName() const override {
        return "TestEmittersScene";
    }
};
