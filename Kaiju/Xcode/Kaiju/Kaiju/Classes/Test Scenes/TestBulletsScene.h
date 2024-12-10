#pragma once

#include "SDKIncludes.h"

using namespace PJ;

class TestBulletsScene : public Scene {
public:
    TestBulletsScene() {}

    void LoadInto(WorldNode& root) override {
        root.name = "TestBulletsScene";

        StandardRandom random;

        //        Emitter::SpawnFunc spawnFunc = [](auto& emitter, auto model) {
        //            return Emitter::SpawnList{};
        //        };
        //        UP<SomeDriver> up = NEW<TimerDriver>(0.1f, RunType::Repeat,
        //        []() {}); MAKE<Emitter>(spawnFunc, up);

        QB(root)
            .OrthoStandard()
            .And("Emitter")
            .Turn(Angle::DegreesAngle(10))
            .With<Emitter>()
            .ModifyLatest<Emitter>([&root](auto& emitter) {
                emitter.spawnParent = SCAST<WorldNode>(root.shared_from_this());

                emitter.spawnFunc = [](Emitter& emitter, EmitModel emit) {
                    auto owner = emitter.owner;
                    auto node = MAKE<WorldNode>("Bullet");

                    auto texture = DCAST<GLTexture>(owner->World()->FindTexture("heart-full"));
                    GUARDR(texture, Emitter::SpawnList{})
                    auto material = MAKE<RenderMaterial>();
                    material->SetShaderProgram(SomeShaderProgram::registry["texture.uniform"]);
                    material->EnableFeature(RenderFeature::Blend, true);
                    material->Add(texture);

                    auto spriteRenderer = MAKE<SpriteRenderer>(material);
                    node->Add(spriteRenderer);

                    auto angle = emit.tags.SafeValue<Angle>(EmitModelTag::Angle);
                    node->transform.SetRotation(angle + emitter.owner->transform.Rotation2D());

                    auto steering = MAKE<DirectionKSteering2D>(300);
                    node->Add(steering);

                    node->Destroy(4);

                    return Emitter::SpawnList{ node };
                };
                emitter.driver =
                    NEW<TimerDriver>(0.5f, RunType::Repeat, [&emitter]() { emitter.Fire(); });

                emitter.emitFunc = EmitFuncs::MakeSpread2D(
                    4, Angle::DegreesAngle(20.0f), Angle::DegreesAngle(3.0f)
                );
            });
    }
};
