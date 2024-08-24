#pragma once

#include <PhaseJump/PhaseJump.h>

using namespace PJ;

class DestroyOnTap : public WorldComponent<>, public SomePointerUIEventsResponder {

    // MARK: SomePointerUIEventsResponder
    
    void OnPointerDown(PointerDownUIEvent event) override {
        GUARD(owner)
        owner->Destroy();
    }
};

class TestEmittersScene : public Scene {
public:
    SP<RenderMaterial> heartMaterial;

    TestEmittersScene() {
    }

    void LoadInto(WorldNode& root) {
        root.name = "TestEmittersScene";

        auto camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());
        auto cameraNode = MAKE<WorldNode>("Camera");
        cameraNode->Add(camera);
        root.Add(cameraNode);
        
//        auto raycaster = MAKE<SimpleRaycaster2D>();
//        cameraNode->Add(raycaster);
//        root.Add(cameraNode);

        World& world = *root.World();

//        auto uiSystem = MAKE<UISystem>();
//        world.Add(uiSystem);

        auto texture = DCAST<GLTexture>(world.loadedResources->map["texture"]["heart-full"].resource);
        GUARD(texture)

        heartMaterial = MAKE<RenderMaterial>();
        heartMaterial->SetShaderProgram(SomeShaderProgram::registry["texture.uniform"]);
        heartMaterial->EnableFeature(RenderFeature::Blend, false);
        heartMaterial->Add(texture);

        auto node = MAKE<WorldNode>("Emitter");
        Emitter::SpawnFunc spawnFunc = [this](Emitter& emitter, EmitModel emit) {
            auto node = MAKE<WorldNode>("Heart");

#define ACID_TEST

            // Used to test batching even if material isn't shared
#ifdef ACID_TEST
            auto texture = DCAST<GLTexture>(owner->World()->loadedResources->map["texture"]["heart-full"].resource);
            auto heartMaterial = MAKE<RenderMaterial>();
            heartMaterial->SetShaderProgram(SomeShaderProgram::registry["texture.uniform"]);
            heartMaterial->EnableFeature(RenderFeature::Blend, false);
            heartMaterial->Add(texture);
#endif
            auto spriteRenderer = MAKE<SpriteRenderer>(heartMaterial);
            node->Add(spriteRenderer);

            ComponentTool ct;
            auto collider = MAKE<PolygonCollider2D>();
            ct.AddComponent(*node, collider);
           
            node->Add(MAKE<DestroyOnTap>());

            auto angle = emit.tags.SafeValue<Angle>(EmitModelTag::Angle2D);
            Vec2 angleVector = (Vector2)angle;
            Vec3 velocity = angleVector * 30;

            auto steering = MAKE<VelocityKSteering>(velocity);
            node->Add(steering);

            return std::optional<Emitter::SpawnType>(node);
        };
        auto emitter = MAKE<Emitter>(spawnFunc, .1f);
//        emitter->maxAlive = 5;

        emitter->buildEmitsFunc = StandardEmitsBuilder::MakeSpread2DFunc(4, Angle::DegreesAngle(20.0f), Angle::DegreesAngle(3.0f));
        emitter->positionFunc = CirclePositioner2D::MakeFunc(500.0f);

        node->Add(emitter);
        root.Add(node);
    }
};
