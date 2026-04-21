#include "gtest/gtest.h"

#include "OrthoCamera.h"
#include "Prefab.h"
#include "Viewport.h"
#include "World.h"
#include "WorldNode.h"
#include "MockRenderContext.h"
#include "MockRenderer.h"

using namespace std;
using namespace PJ;
using namespace PJTest;

namespace WorldTests {
    class TestWorld : public World {
    public:
        using Base = World;
        
        int renderNowCount{};
        
        void SetIsRemoveNodesLocked(bool value) {
            isRemoveNodesLockedCount = value ? 1 : 0;
        }
        
        void RenderNow() override {
            Base::RenderNow();
            renderNowCount++;
        }
    };
    
    class TestWorldNode : public WorldNode {
    public:
        float updateTime{};
        
        FinishType OnUpdate(TimeSlice time) override {
            updateTime += time.delta;
            return WorldNode::OnUpdate(time);
        }
    };
    
    class TestWorldSystem : public WorldSystem {
    public:
        float updateTime{};
        
        FinishType OnUpdate(TimeSlice time) override {
            updateTime += time.delta;
            return WorldSystem::OnUpdate(time);
        }
    };
}

using namespace WorldTests;

TEST(World, Init) {
    World sut;
    EXPECT_EQ(sut.Root()->World(), &sut);
}

TEST(World, PausePlay) {
    TestWorld sut;
    
    EXPECT_FALSE(sut.IsPaused());
    sut.Pause();
    EXPECT_TRUE(sut.IsPaused());
    sut.Play();
    EXPECT_FALSE(sut.IsPaused());
}

TEST(World, RenderNow) {
    TestWorld sut;
    
    EXPECT_EQ(0, sut.renderNowCount);
    sut.RenderNow();
    EXPECT_EQ(1, sut.renderNowCount);
    sut.RenderNow();
}

TEST(World, RenderCountRateLimited) {
    TestWorld sut;
    
    EXPECT_EQ(0, sut.renderNowCount);
    
    sut.SetIsRenderRateLimited(true);
    float fps = 30.0f;
    sut.SetRenderRate(fps);
    
    sut.Render();
    EXPECT_EQ(1, sut.renderNowCount);
    
    sut.Render();
    EXPECT_EQ(1, sut.renderNowCount);
    
    sut.OnUpdate(1.0f/fps);
    sut.Render();
    EXPECT_EQ(2, sut.renderNowCount);
}

TEST(World, AddSystem) {
    World sut;
    auto system1 = MAKE<WorldSystem>();
    sut.Add(system1);
    
    EXPECT_EQ(system1->World(), &sut);
    ASSERT_EQ(1, sut.Systems().size());
    EXPECT_EQ(system1, sut.Systems()[0]);
}

TEST(World, RemoveSystem) {
    World sut;
    auto system1 = MAKE<WorldSystem>();
    auto system2 = MAKE<WorldSystem>();
    sut.Add(system1);
    sut.Add(system2);
    
    EXPECT_EQ(system1->World(), &sut);
    EXPECT_EQ(system2->World(), &sut);
    EXPECT_EQ(2, sut.Systems().size());
    
    sut.Remove(*system2);
    EXPECT_EQ(nullptr, system2->World());
    EXPECT_EQ(1, sut.Systems().size());
}

TEST(World, RemoveAllSystems) {
    World sut;
    auto system1 = MAKE<WorldSystem>();
    auto system2 = MAKE<WorldSystem>();
    sut.Add(system1);
    sut.Add(system2);
    
    EXPECT_EQ(system1->World(), &sut);
    EXPECT_EQ(system2->World(), &sut);
    EXPECT_EQ(2, sut.Systems().size());
    
    sut.RemoveAllSystems();
    EXPECT_EQ(nullptr, system1->World());
    EXPECT_EQ(nullptr, system2->World());
    EXPECT_EQ(0, sut.Systems().size());
}

TEST(World, MainCamera) {
    TestWorld world;
    
    auto node = MAKE<WorldNode>();
    world.Add(node);
    auto& camera = node->With<OrthoCamera>();
    
    auto mainCamera = world.MainCamera();
    EXPECT_EQ(&camera, mainCamera);
}

TEST(World, MainCameraWithOffscreen) {
    TestWorld world;
    
    auto node = MAKE<WorldNode>();
    world.Add(node);
    auto& camera = node->With<OrthoCamera>();
    camera.renderContext = MAKE<MockRenderContext>(MockRenderContext::Config{});
    
    auto mainCamera = world.MainCamera();
    EXPECT_EQ(nullptr, mainCamera);
}

TEST(World, AddNode) {
    TestWorld world;
    
    auto node = MAKE<WorldNode>();
    world.Add(node);
    world.Go();
    
    WorldNode* root = world.Root();
    
    ASSERT_EQ(1, root->ChildNodes().size());
    WorldNode* firstNode = root->ChildNodes()[0].get();
    EXPECT_EQ(firstNode, node.get());
    EXPECT_EQ(firstNode->Parent(), root);
    EXPECT_EQ(firstNode->World(), &world);
}

TEST(World, RemoveAllNodes) {
    TestWorld world;

    auto node = MAKE<WorldNode>();
    world.Add(node);
    world.Go();
    
    WorldNode* root = world.Root();

    ASSERT_EQ(1, root->ChildNodes().size());
    WorldNode* firstNode = root->ChildNodes()[0].get();
    EXPECT_EQ(firstNode, node.get());
    EXPECT_EQ(firstNode->Parent(), root);
    EXPECT_EQ(firstNode->World(), &world);
    
    world.RemoveAllNodes();
    
    ASSERT_EQ(0, root->ChildNodes().size());
    EXPECT_EQ(firstNode->Parent(), nullptr);
    EXPECT_EQ(firstNode->World(), nullptr);
}

TEST(World, RemoveAllNodesWithRemoveLocked) {
    TestWorld world;
    
    auto node = MAKE<WorldNode>();
    world.Add(node);
    world.Go();
    
    WorldNode* root = world.Root();
    
    world.SetIsRemoveNodesLocked(true);
    
    ASSERT_EQ(1, root->ChildNodes().size());
    WorldNode* firstNode = root->ChildNodes()[0].get();
    EXPECT_EQ(firstNode, node.get());
    EXPECT_EQ(firstNode->Parent(), root);
    EXPECT_EQ(firstNode->World(), &world);
    
    world.RemoveAllNodes();
        
    ASSERT_EQ(1, root->ChildNodes().size());
    firstNode = root->ChildNodes()[0].get();
    EXPECT_EQ(firstNode, node.get());
    EXPECT_EQ(firstNode->Parent(), root);
    EXPECT_EQ(firstNode->World(), &world);
}

TEST(World, DestroyAllNodes) {
    TestWorld world;
    
    auto node = MAKE<WorldNode>();
    world.Add(node);
    world.Go();
    
    WorldNode* root = world.Root();
    
    world.DestroyAllNodes();
    
    for (auto& child : root->Children()) {
        EXPECT_TRUE(child->IsDestroyed());
    }
}

TEST(World, DestroyRemovesDuringUpdate) {
    TestWorld world;
    
    auto node = MAKE<WorldNode>();
    world.Add(node);
    world.Go();
    
    WorldNode* root = world.Root();
    
    node->Destroy();
    EXPECT_TRUE(node->IsDestroyed());
    EXPECT_EQ(node->Parent(), root);
    
    world.OnUpdate({10});
    EXPECT_EQ(node->Parent(), nullptr);
    EXPECT_EQ(node->World(), nullptr);
    EXPECT_EQ(0, root->ChildCount());
}

TEST(World, TestNodePositionsAtRoot) {
    UP<World> world = NEW<World>();

    auto node = MAKE<WorldNode>();
    world->Add(node);
    world->Go();

    EXPECT_EQ(Vector3{}, node->transform.LocalPosition());
    EXPECT_EQ(Vector3{}, node->transform.WorldPosition());

    node->transform.SetLocalPosition(Vector3(1, 1, 0));
    EXPECT_EQ(Vector3(1, 1, 0), node->transform.LocalPosition());
    EXPECT_EQ(Vector3(1, 1, 0), node->transform.WorldPosition());
}

TEST(World, TestNodePositionsAsChild) {
    UP<World> world = NEW<World>();

    auto node = MAKE<WorldNode>();
    auto parentNode = MAKE<WorldNode>();
    parentNode->Add(node);
    world->Add(parentNode);
    world->Go();

    EXPECT_EQ(Vector3{}, node->transform.LocalPosition());
    EXPECT_EQ(Vector3{}, node->transform.WorldPosition());

    node->transform.SetLocalPosition(Vector3(1, 1, 0));
    EXPECT_EQ(Vector3(1, 1, 0), node->transform.LocalPosition());
    EXPECT_EQ(Vector3(1, 1, 0), node->transform.WorldPosition());

    parentNode->transform.SetLocalPosition(Vec3(10, 10, 0));
    EXPECT_EQ(Vector3(1, 1, 0), node->transform.LocalPosition());
    EXPECT_EQ(Vector3(11, 11, 0), node->transform.WorldPosition());
}

TEST(World, OnUpdate) {
    TestWorld world;
    
    auto node = MAKE<TestWorldNode>();
    world.Add(node);
    world.Go();
    
    auto system = MAKE<TestWorldSystem>();
    world.Add(system);
    
    EXPECT_FALSE(node->IsAwake());
    EXPECT_FALSE(node->IsStarted());
    EXPECT_FALSE(system->IsAwake());
    EXPECT_FALSE(system->IsStarted());
    
    world.OnUpdate(1);
    
    EXPECT_TRUE(node->IsAwake());
    EXPECT_TRUE(node->IsStarted());
    EXPECT_TRUE(system->IsAwake());
    EXPECT_TRUE(system->IsStarted());
    
    EXPECT_EQ(1, node->updateTime);
    EXPECT_EQ(1, system->updateTime);
}

TEST(World, WorldToScreen) {
    TestWorld world;
    
    auto node = MAKE<TestWorldNode>();
    world.Add(node);
    world.Go();
    world.renderContext = NEW<MockRenderContext>(MockRenderContext::Config{.size = {400, 200}, .pixelSize = {400, 200}});

    node->With<OrthoCamera>();

    auto expectedPos = Vector2{200, 100};
    auto worldPos = Vector3{0, 0, 0};
    auto screenPos = world.WorldToScreen(worldPos);
    EXPECT_EQ(expectedPos, screenPos);
}

TEST(World, ScreenToLocal) {
    TestWorld world;
    
    auto node = MAKE<TestWorldNode>();
    world.Add(node);
    world.Go();
    world.renderContext = NEW<MockRenderContext>(MockRenderContext::Config{.size = {400, 200}, .pixelSize = {400, 200}});

    node->With<OrthoCamera>();

    auto& component = node->With<WorldComponent>();

    auto screenPos = Vector2{201, 101};
    auto expectedPos = Vector3{1, -1, 0};
    EXPECT_EQ(expectedPos, ScreenToLocal(component, screenPos));
}

TEST(World, ParentWorldModelMatrix_NoParent_ReturnsIdentity) {
    auto world = MAKE<World>();
    auto node = MAKE<WorldNode>();

    auto matrix = world->ParentWorldModelMatrix(*node);
    EXPECT_EQ(Vector3(0, 0, 0), matrix.MultiplyPoint(Vector3(0, 0, 0)));
    EXPECT_EQ(Vector3(7, -3, 2), matrix.MultiplyPoint(Vector3(7, -3, 2)));
}

TEST(World, ParentWorldModelMatrix_AppliesParentTranslation) {
    auto world = MAKE<World>();
    auto parent = MAKE<WorldNode>();
    world->Add(parent);
    parent->SetLocalPosition(Vector3(10, 20, 0));

    auto child = MAKE<WorldNode>();
    parent->Add(child);

    auto matrix = world->ParentWorldModelMatrix(*child);
    EXPECT_EQ(Vector3(10, 20, 0), matrix.MultiplyPoint(Vector3(0, 0, 0)));
    EXPECT_EQ(Vector3(11, 20, 0), matrix.MultiplyPoint(Vector3(1, 0, 0)));
}

TEST(World, ParentWorldModelMatrix_StacksAncestorTranslations) {
    auto world = MAKE<World>();
    auto grandparent = MAKE<WorldNode>();
    world->Add(grandparent);
    grandparent->SetLocalPosition(Vector3(5, 0, 0));

    auto parent = MAKE<WorldNode>();
    grandparent->Add(parent);
    parent->SetLocalPosition(Vector3(2, 3, 0));

    auto child = MAKE<WorldNode>();
    parent->Add(child);

    auto matrix = world->ParentWorldModelMatrix(*child);
    EXPECT_EQ(Vector3(7, 3, 0), matrix.MultiplyPoint(Vector3(0, 0, 0)));
}

TEST(World, ParentWorldModelMatrix_AppliesParentScale) {
    auto world = MAKE<World>();
    auto parent = MAKE<WorldNode>();
    world->Add(parent);
    parent->SetLocalPosition(Vector3(10, 0, 0));
    parent->SetScale(Vector3(2, 2, 1));

    auto child = MAKE<WorldNode>();
    parent->Add(child);

    auto matrix = world->ParentWorldModelMatrix(*child);
    // Scale applies first, then translate: (1,0,0) -> (2,0,0) -> (12,0,0).
    EXPECT_EQ(Vector3(12, 0, 0), matrix.MultiplyPoint(Vector3(1, 0, 0)));
}

TEST(World, ParentWorldModelMatrix_ViewportParentIsTranslateOnly) {
    auto world = MAKE<World>();
    auto viewportNode = MAKE<WorldNode>();
    world->Add(viewportNode);
    viewportNode->AddComponent<Viewport>();
    viewportNode->SetLocalPosition(Vector3(10, 0, 0));
    // Scale must be ignored because the Viewport contributes translation-only to descendants.
    viewportNode->SetScale(Vector3(3, 3, 1));

    auto child = MAKE<WorldNode>();
    viewportNode->Add(child);

    auto matrix = world->ParentWorldModelMatrix(*child);
    EXPECT_EQ(Vector3(11, 0, 0), matrix.MultiplyPoint(Vector3(1, 0, 0)));
}

TEST(World, ParentWorldModelMatrix_ViewportCollapsesAncestorsAboveIt) {
    auto world = MAKE<World>();
    auto grandparent = MAKE<WorldNode>();
    world->Add(grandparent);
    grandparent->SetLocalPosition(Vector3(10, 0, 0));
    // Grandparent's scale should also be ignored once a Viewport is found below it.
    grandparent->SetScale(Vector3(2, 2, 1));

    auto viewportNode = MAKE<WorldNode>();
    grandparent->Add(viewportNode);
    viewportNode->AddComponent<Viewport>();
    viewportNode->SetLocalPosition(Vector3(5, 0, 0));

    auto child = MAKE<WorldNode>();
    viewportNode->Add(child);

    auto matrix = world->ParentWorldModelMatrix(*child);
    // Pure translate stack: viewport (5) + grandparent (10) = 15; input x passes through.
    EXPECT_EQ(Vector3(16, 0, 0), matrix.MultiplyPoint(Vector3(1, 0, 0)));
}

TEST(World, LocalModelMatrix_AppliesTranslation) {
    auto world = MAKE<World>();
    auto node = MAKE<WorldNode>();
    node->SetLocalPosition(Vector3(5, -3, 2));

    auto matrix = world->LocalModelMatrix(*node);
    EXPECT_EQ(Vector3(5, -3, 2), matrix.MultiplyPoint(Vector3(0, 0, 0)));
    EXPECT_EQ(Vector3(6, -3, 2), matrix.MultiplyPoint(Vector3(1, 0, 0)));
}

TEST(World, LocalModelMatrix_AppliesScale) {
    auto world = MAKE<World>();
    auto node = MAKE<WorldNode>();
    node->SetScale(Vector3(3, 2, 1));

    auto matrix = world->LocalModelMatrix(*node);
    EXPECT_EQ(Vector3(0, 0, 0), matrix.MultiplyPoint(Vector3(0, 0, 0)));
    EXPECT_EQ(Vector3(3, 4, 0), matrix.MultiplyPoint(Vector3(1, 2, 0)));
}

TEST(World, LocalModelMatrix_CombinesTranslateAndScale) {
    auto world = MAKE<World>();
    auto node = MAKE<WorldNode>();
    node->SetLocalPosition(Vector3(10, 0, 0));
    node->SetScale(Vector3(2, 2, 1));

    // Scale first, then translate: (1,0,0) -> (2,0,0) -> (12,0,0).
    auto matrix = world->LocalModelMatrix(*node);
    EXPECT_EQ(Vector3(12, 0, 0), matrix.MultiplyPoint(Vector3(1, 0, 0)));
}

TEST(World, WorldModelMatrix_NoParent_UsesLocalMatrix) {
    auto world = MAKE<World>();
    auto node = MAKE<WorldNode>();
    node->SetLocalPosition(Vector3(4, 7, 0));

    // With no parent, WorldModelMatrix == LocalModelMatrix.
    auto matrix = world->WorldModelMatrix(*node);
    EXPECT_EQ(Vector3(4, 7, 0), matrix.MultiplyPoint(Vector3(0, 0, 0)));
    EXPECT_EQ(Vector3(5, 7, 0), matrix.MultiplyPoint(Vector3(1, 0, 0)));
}

TEST(World, WorldModelMatrix_CombinesNodeAndParentTransforms) {
    auto world = MAKE<World>();
    auto parent = MAKE<WorldNode>();
    world->Add(parent);
    parent->SetLocalPosition(Vector3(10, 0, 0));

    auto node = MAKE<WorldNode>();
    parent->Add(node);
    node->SetLocalPosition(Vector3(3, 2, 0));

    // Node's local (1,0,0) -> (4,2,0) in parent space -> (14,2,0) in world space.
    auto matrix = world->WorldModelMatrix(*node);
    EXPECT_EQ(Vector3(14, 2, 0), matrix.MultiplyPoint(Vector3(1, 0, 0)));
}

TEST(World, WorldModelMatrix_ViewportDropsAncestorScaleButKeepsNodeLocal) {
    auto world = MAKE<World>();
    auto viewportNode = MAKE<WorldNode>();
    world->Add(viewportNode);
    viewportNode->AddComponent<Viewport>();
    // Ancestor viewport's scale must not propagate to the node.
    viewportNode->SetScale(Vector3(5, 5, 1));
    viewportNode->SetLocalPosition(Vector3(10, 0, 0));

    auto node = MAKE<WorldNode>();
    viewportNode->Add(node);
    // The node's own local scale DOES apply — only ancestors up through the viewport
    // are collapsed to translation-only.
    node->SetScale(Vector3(2, 2, 1));

    auto matrix = world->WorldModelMatrix(*node);
    // Node scale 2x on (1,0,0) = (2,0,0); viewport translate adds 10; grandparent none.
    EXPECT_EQ(Vector3(12, 0, 0), matrix.MultiplyPoint(Vector3(1, 0, 0)));
}

TEST(World, MainCamera_CachesResult) {
    TestWorld world;

    auto node = MAKE<WorldNode>();
    world.Add(node);
    auto& camera = node->With<OrthoCamera>();

    auto first = world.MainCamera();
    auto second = world.MainCamera();
    EXPECT_EQ(&camera, first);
    EXPECT_EQ(first, second);
}

TEST(World, MainCamera_InvalidatesWhenOwnerRemoved) {
    TestWorld world;

    auto node = MAKE<WorldNode>();
    world.Add(node);
    node->With<OrthoCamera>();

    EXPECT_NE(nullptr, world.MainCamera());

    world.Root()->Remove(*node);
    node.reset();

    // After the owning node is gone, the cached weak_ptr expires and MainCamera re-walks.
    EXPECT_EQ(nullptr, world.MainCamera());
}

TEST(World, MainCamera_RejectsCameraReParentedToOtherWorld) {
    auto world1 = MAKE<World>();
    auto world2 = MAKE<World>();

    auto cameraNode = MAKE<WorldNode>();
    world1->Add(cameraNode);
    auto& camera = cameraNode->With<OrthoCamera>();

    // Prime world1's cache.
    EXPECT_EQ(&camera, world1->MainCamera());

    // Re-parent the camera node to world2.
    world1->Root()->Remove(*cameraNode);
    world2->Add(cameraNode);

    // world1 must NOT return the foreign camera even though its weak_ptr is still alive.
    EXPECT_EQ(nullptr, world1->MainCamera());
    // world2 now owns it.
    EXPECT_EQ(&camera, world2->MainCamera());
}

TEST(World, TypeSystem_Found) {
    World sut;
    auto system = MAKE<TestWorldSystem>();
    sut.Add(system);

    EXPECT_EQ(system.get(), sut.TypeSystem<TestWorldSystem>());
}

TEST(World, TypeSystem_NotFound) {
    World sut;
    sut.Add(MAKE<WorldSystem>());

    EXPECT_EQ(nullptr, sut.TypeSystem<TestWorldSystem>());
}

TEST(World, LoadPrefab_InvokesLoadFunc) {
    World sut;
    auto node = MAKE<WorldNode>();
    sut.Add(node);

    int loadCount = 0;
    auto prefab = MAKE<Prefab>([&](WorldNode& target) { loadCount++; });
    sut.prefabs["test"] = prefab;

    sut.LoadPrefab("test", *node);
    EXPECT_EQ(1, loadCount);
}

TEST(World, LoadPrefab_UnknownIdIsNoOp) {
    World sut;
    auto node = MAKE<WorldNode>();
    sut.Add(node);

    // Must not throw or crash on an unknown prefab id.
    sut.LoadPrefab("missing", *node);
}

TEST(World, ScreenToWorld_ComponentVariant) {
    TestWorld world;

    auto node = MAKE<WorldNode>();
    world.Add(node);
    world.Go();
    world.renderContext = NEW<MockRenderContext>(
        MockRenderContext::Config{ .size = { 400, 200 }, .pixelSize = { 400, 200 } }
    );

    node->With<OrthoCamera>();
    auto& component = node->With<WorldComponent>();

    // Screen (200,100) is the render-context center, which maps to world origin.
    EXPECT_EQ(Vector3(0, 0, 0), ScreenToWorld(component, Vector2(200, 100)));
    // (201,101) is one pixel right and one pixel DOWN; world Y is up, so Y flips.
    EXPECT_EQ(Vector3(1, -1, 0), ScreenToWorld(component, Vector2(201, 101)));
}

TEST(World, WorldToScreen_WorldNodeVariant) {
    TestWorld world;

    // Camera on its own node so it doesn't follow the node we're querying.
    auto cameraNode = MAKE<WorldNode>();
    world.Add(cameraNode);
    cameraNode->With<OrthoCamera>();

    auto node = MAKE<WorldNode>();
    world.Add(node);
    world.Go();
    world.renderContext = NEW<MockRenderContext>(
        MockRenderContext::Config{ .size = { 400, 200 }, .pixelSize = { 400, 200 } }
    );

    // Node at world origin renders at screen center.
    EXPECT_EQ(Vector2(200, 100), WorldToScreen(*node));

    node->transform.SetLocalPosition(Vector3(10, 5, 0));
    // World (10,5) -> screen (210, 95) because Y flips around the vertical center.
    EXPECT_EQ(Vector2(210, 95), WorldToScreen(*node));
}

TEST(World, LocalToScreen_RoundTripsWithScreenToLocal) {
    TestWorld world;

    auto node = MAKE<WorldNode>();
    world.Add(node);
    world.Go();
    world.renderContext = NEW<MockRenderContext>(
        MockRenderContext::Config{ .size = { 400, 200 }, .pixelSize = { 400, 200 } }
    );

    node->With<OrthoCamera>();
    auto& component = node->With<WorldComponent>();

    Vector2 screenPos(201, 101);
    auto localPos = ScreenToLocal(component, screenPos);
    EXPECT_EQ(screenPos, LocalToScreen(component, localPos));
}
