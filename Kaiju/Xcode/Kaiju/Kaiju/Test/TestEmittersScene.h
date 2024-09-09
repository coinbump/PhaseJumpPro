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
       
//#define ADD_RAYCASTER

#ifdef ADD_RAYCASTER
        auto raycaster = MAKE<SimpleRaycaster2D>();
        cameraNode->Add(raycaster);
        root.Add(cameraNode);
#endif

        World& world = *root.World();


        auto texture = DCAST<GLTexture>(world.loadedResources->map["texture"]["heart-full"].resource);
        GUARD(texture)

        heartMaterial = MAKE<RenderMaterial>();
        heartMaterial->SetShaderProgram(SomeShaderProgram::registry["texture.uniform"]);
        heartMaterial->EnableFeature(RenderFeature::Blend, false);
        heartMaterial->Add(texture);

        auto font = DCAST<Font>(world.loadedResources->map["font"]["ArialBlack-32"].resource);

        auto node = MAKE<WorldNode>("Emitter");
        Emitter::SpawnFunc spawnFunc = [this, font](Emitter& emitter, EmitModel emit) {
            auto node = MAKE<WorldNode>("Heart");

//#define ACID_TEST

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

            auto countString = MakeString(emitter.owner->ChildCount());

            WorldNode& textNode = node->AddNode("Text " + countString);
            textNode.AddComponent<TextRenderer>(font, countString, Vector2(400, 400))
                .SetLineAlignFunc(AlignFuncs::center)
                .SetTextAlignFunc(AlignFuncs::center)
                .SetTextColor(Color::white)
                .SetLineClip(LineClip::None)
                .SizeToFit();

//#define DESTROY_ON_TAP

#ifdef DESTROY_ON_TAP
            ComponentTool ct;
            auto collider = MAKE<PolygonCollider2D>();
            ct.AddComponent(*node, collider);
           
            node->Add(MAKE<DestroyOnTap>());
#endif

            auto angle = emit.tags.SafeValue<Angle>(EmitModelTag::Angle2D);
            Vec2 angleVector = (Vector2)angle;
            Vec3 velocity = angleVector * 30;

            auto steering = MAKE<VelocityKSteering>(velocity);
            node->Add(steering);
            auto lp = node->transform.LocalPosition();
            lp.z = node->ChildCount() * -.001f;
            node->transform.SetLocalPosition(lp);

            return std::optional<Emitter::SpawnType>(node);
        };
        auto emitter = MAKE<Emitter>(spawnFunc, .1f);

//#define MAX_ALIVE

#ifdef MAX_ALIVE
        emitter->maxAlive = 5;
#endif

        emitter->buildEmitsFunc = StandardEmitsBuilder::MakeSpread2DFunc(4, Angle::DegreesAngle(20.0f), Angle::DegreesAngle(3.0f));
        emitter->positionFunc = CirclePositioner2D::MakeFunc(500.0f);

        node->Add(emitter);
        root.Add(node);
    }
};
