#include "gtest/gtest.h"

#include "OrthoCamera.h"
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

// FUTURE: Unit test LocalModelMatrix, WorldModelMatrix
