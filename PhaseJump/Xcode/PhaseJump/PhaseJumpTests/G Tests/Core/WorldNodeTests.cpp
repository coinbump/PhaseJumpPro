#include "gtest/gtest.h"
#include "WorldNode.h"
#include "WorldComponent.h"
#include "World.h"
#include "StringUtils.h"

using namespace PJ;
using namespace std;

namespace WorldNodeTests {
    class TestComponent : public WorldComponent<>
    {
    public:
        int destroyCount = 0;
        float updateTime = 0;

        void OnDestroy() override {
            destroyCount++;
        }

        void OnUpdate(TimeSlice time) override {
            updateTime += time.delta;
        }
    };
}

using namespace WorldNodeTests;

TEST(WorldNode, Toggles)
{
    WorldNode sut("name");
    EXPECT_EQ(String("name"), sut.name);
    EXPECT_TRUE(sut.IsEnabled());
    EXPECT_FALSE(sut.IsAwake());
    EXPECT_FALSE(sut.IsStarted());
    EXPECT_FALSE(sut.IsDestroyed());

    sut.ToggleEnable();
    EXPECT_FALSE(sut.IsEnabled());

    sut.Destroy();
    EXPECT_TRUE(sut.IsDestroyed());
}

TEST(WorldNode, Lifecycle)
{
    WorldNode sut("");
    EXPECT_FALSE(sut.IsAwake());
    EXPECT_FALSE(sut.IsStarted());

    sut.CheckedAwake();
    EXPECT_TRUE(sut.IsAwake());
    EXPECT_FALSE(sut.IsStarted());

    sut.CheckedStart();
    EXPECT_TRUE(sut.IsAwake());
    EXPECT_TRUE(sut.IsStarted());
}

TEST(WorldNode, AddComponent)
{
    WorldNode sut("");
    auto component = MAKE<WorldComponent<>>();
    sut.Add(component);

    EXPECT_EQ(&sut, component->Node());
    EXPECT_EQ(1, sut.Components().size());
}

TEST(WorldNode, AddComponent_Lifecycle)
{
    WorldNode sut("");
    sut.CheckedAwake();
    sut.CheckedStart();

    auto component = MAKE<WorldComponent<>>();
    sut.Add(component);

    EXPECT_TRUE(component->IsAwake());
    EXPECT_TRUE(component->IsStarted());
}

TEST(WorldNode, AddComponent_AlreadyParented)
{
    WorldNode sut("");
    WorldNode sut2("");

    auto component = MAKE<WorldComponent<>>();
    sut.Add(component);

    sut2.Add(component);
    EXPECT_EQ(&sut, component->Node());
    EXPECT_EQ(1, sut.Components().size());
    EXPECT_EQ(0, sut2.Components().size());
}

TEST(WorldNode, AddComponent_Null)
{
    WorldNode sut("");
    WorldNode sut2("");

    SP<WorldComponent<>> component;
    sut.Add(component);
    EXPECT_EQ(0, sut.Components().size());
}

TEST(WorldNode, AddComponentT)
{
    WorldNode sut("");
    sut.AddComponent<WorldComponent<>>();
    EXPECT_EQ(1, sut.Components().size());
}

TEST(WorldNode, AddNode)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);
    EXPECT_EQ(world.get(), sut->World());

    auto child = MAKE<WorldNode>();

    sut->Add(child);
    EXPECT_EQ(world.get(), child->World());
    EXPECT_EQ(sut.get(), child->Parent());
    EXPECT_EQ(1, sut->ChildCount());
}

TEST(WorldNode, AddNode_Deep)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);
    EXPECT_EQ(world.get(), sut->World());

    auto child = MAKE<WorldNode>();
    auto child2 = MAKE<WorldNode>();
    child->Add(child2);

    sut->Add(child);
    EXPECT_EQ(world.get(), child->World());
    EXPECT_EQ(sut.get(), child->Parent());
    EXPECT_EQ(1, sut->ChildCount());
    EXPECT_EQ(world.get(), child2->World());
    EXPECT_EQ(child.get(), child2->Parent());
    EXPECT_EQ(1, child->ChildCount());
}

TEST(WorldNode, AddNode_AlreadyParented)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    auto sut2 = MAKE<WorldNode>();
    world->Add(sut);

    auto child = MAKE<WorldNode>();

    sut->Add(child);
    sut2->Add(child);
    EXPECT_EQ(1, sut->ChildCount());
    EXPECT_EQ(0, sut2->ChildCount());
}

TEST(WorldNode, AddNode_Null)
{
    auto world = MAKE<World>();
    SP<WorldNode> sut;
    world->Add(sut);
    EXPECT_EQ(0, world->root->ChildCount());
}

TEST(WorldNode, RemoveNode)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);
    EXPECT_EQ(world.get(), sut->World());

    auto child = MAKE<WorldNode>();

    sut->Add(child);
    EXPECT_EQ(world.get(), child->World());
    EXPECT_EQ(sut.get(), child->Parent());
    EXPECT_EQ(1, sut->ChildCount());

    sut->Remove(*child);
    EXPECT_EQ(nullptr, child->World());
    EXPECT_EQ(nullptr, child->Parent());
    EXPECT_EQ(0, sut->ChildCount());
}

TEST(WorldNode, RemoveNode_Deep)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);
    EXPECT_EQ(world.get(), sut->World());

    auto child = MAKE<WorldNode>();
    auto child2 = MAKE<WorldNode>();
    child->Add(child2);

    sut->Add(child);
    sut->Remove(*child);
    EXPECT_EQ(nullptr, child->World());
    EXPECT_EQ(nullptr, child->Parent());
    EXPECT_EQ(0, sut->ChildCount());
    EXPECT_EQ(nullptr, child2->World());
    EXPECT_EQ(child.get(), child2->Parent());
    EXPECT_EQ(1, child->ChildCount());
}

TEST(WorldNode, RemoveAllChildren)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);
    EXPECT_EQ(world.get(), sut->World());

    auto child = MAKE<WorldNode>();
    auto child2 = MAKE<WorldNode>();
    child->Add(child2);

    sut->Add(child);
    sut->RemoveAllChildren();
    EXPECT_EQ(nullptr, child->World());
    EXPECT_EQ(nullptr, child->Parent());
    EXPECT_EQ(0, sut->ChildCount());
    EXPECT_EQ(nullptr, child2->World());
    EXPECT_EQ(child.get(), child2->Parent());
    EXPECT_EQ(1, child->ChildCount());
}

TEST(WorldNode, RemoveComponent)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);

    auto component = MAKE<WorldComponent<>>();
    sut->Add(component);
    EXPECT_EQ(1, sut->Components().size());
    EXPECT_EQ(sut.get(), component->owner);
    sut->Remove(*component);
    EXPECT_EQ(0, sut->Components().size());
    EXPECT_EQ(nullptr, component->owner);
}

TEST(WorldNode, RemoveAllComponents)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);

    auto component = MAKE<WorldComponent<>>();
    sut->Add(component);
    auto component2 = MAKE<WorldComponent<>>();
    sut->Add(component2);
    EXPECT_EQ(2, sut->Components().size());
    sut->RemoveAllComponents();
    EXPECT_EQ(0, sut->Components().size());
    EXPECT_EQ(nullptr, component->owner);
    EXPECT_EQ(nullptr, component2->owner);
}

TEST(WorldNode, RemoveTypeComponents)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);

    auto component = MAKE<TestComponent>();
    sut->Add(component);
    auto component2 = MAKE<WorldComponent<>>();
    sut->Add(component2);
    EXPECT_EQ(2, sut->Components().size());
    sut->RemoveType<TestComponent>();
    EXPECT_EQ(1, sut->Components().size());
    EXPECT_EQ(nullptr, component->owner);
    EXPECT_EQ(sut.get(), component2->owner);
}

TEST(WorldNode, Modify)
{
    auto sut = MAKE<WorldNode>();
    sut->Add(MAKE<TestComponent>());
    sut->Add(MAKE<WorldComponent<>>());

    int count{};
    sut->Modify<TestComponent>([&](auto& component) {
        count++;
    });

    EXPECT_EQ(1, count);
}

TEST(WorldNode, TypeComponent)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);

    auto component = MAKE<TestComponent>();
    sut->Add(component);
    auto component2 = MAKE<TestComponent>();
    sut->Add(component2);
    EXPECT_EQ(component, sut->TypeComponent<TestComponent>());
}

TEST(WorldNode, TypeComponent2)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);

    auto component = MAKE<TestComponent>();
    sut->Add(component);
    auto component2 = MAKE<TestComponent>();
    sut->Add(component2);
    List<SP<TestComponent>> components;
    sut->CollectTypeComponents<TestComponent>(components);

    EXPECT_EQ(2, components.size());
    EXPECT_EQ(component, *components.begin());
}

TEST(WorldNode, DestroyNow)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);

    auto component = MAKE<TestComponent>();
    sut->Add(component);
    auto component2 = MAKE<TestComponent>();
    sut->Add(component2);

    sut->Destroy();
    EXPECT_TRUE(sut->IsDestroyed());

    sut->OnDestroy();
    EXPECT_EQ(1, component->destroyCount);
    EXPECT_EQ(1, component2->destroyCount);
}

TEST(WorldNode, DestroyLater)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);

    sut->Destroy(3);
    EXPECT_FALSE(sut->IsDestroyed());

    sut->OnUpdate(TimeSlice(3));
    EXPECT_TRUE(sut->IsDestroyed());
}

TEST(WorldNode, Update)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);

    auto component = MAKE<TestComponent>();
    sut->Add(component);
    auto component2 = MAKE<TestComponent>();
    sut->Add(component2);

    sut->OnUpdate(TimeSlice(3.0f));
    EXPECT_EQ(3.0f, component->updateTime);
    EXPECT_EQ(3.0f, component2->updateTime);
}

TEST(WorldNode, CollectChildTypeComponents)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);

    auto child = MAKE<WorldNode>();
    sut->Add(child);

    auto component = MAKE<TestComponent>();
    sut->Add(component);
    auto component2 = MAKE<TestComponent>();
    child->Add(component2);
    List<SP<TestComponent>> components;
    sut->CollectChildTypeComponents<TestComponent>(components);

    EXPECT_EQ(2, components.size());
    EXPECT_EQ(component, *components.begin());
    EXPECT_EQ(component2, *(++components.begin()));
}

TEST(WorldNode, LocalToWorld)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);
    EXPECT_EQ(world.get(), sut->World());

    auto child = MAKE<WorldNode>();
    auto child2 = MAKE<WorldNode>();
    child->Add(child2);
    sut->Add(child);

    sut->SetLocalPosition({0, 10, 0});
    child->SetLocalPosition({4, 0, 0});
    child2->SetLocalPosition({0, 0, 3});

    EXPECT_EQ(Vector3(4, 10, 3), child2->LocalToWorld(Vector3::Uniform(0)));
}

TEST(WorldNode, LocalToWorldScaled)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);
    EXPECT_EQ(world.get(), sut->World());

    auto child = MAKE<WorldNode>();
    auto child2 = MAKE<WorldNode>();
    child->Add(child2);
    sut->Add(child);

    sut->SetLocalPosition({0, 10, 0});
    child->SetLocalPosition({4, 0, 0});
    child->SetScale(Vector3(.5f, .5f, .5f));
    child2->SetLocalPosition({0, 0, 4});

    EXPECT_EQ(Vector3(4, 10, 2), child2->LocalToWorld(Vector3::Uniform(0)));
}

TEST(WorldNode, WorldToLocalScaled)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);
    EXPECT_EQ(world.get(), sut->World());

    auto child = MAKE<WorldNode>();
    auto child2 = MAKE<WorldNode>();
    child->Add(child2);
    sut->Add(child);

    sut->SetLocalPosition({0, 10, 0});
    child->SetLocalPosition({4, 0, 0});
    child->SetScale(Vector3(.5f, .5f, .5f));
    child2->SetLocalPosition({0, 0, 4});

    EXPECT_EQ(Vector3(0, 0, 0), child2->WorldToLocal(Vector3(4, 10, 2)));
}

TEST(WorldNode, OnAddChildNode)
{
    auto world = MAKE<World>();
    auto sut = MAKE<WorldNode>();
    world->Add(sut);

    int value = 0;
    auto component = MAKE<WorldComponent<StandardCore>>();
    component->signalFuncs[SignalId::AddChildNode] = [&](auto& owner, auto& event) {
        value++;
    };
    sut->Add(component);

    EXPECT_EQ(0, value);

    sut->Add(MAKE<WorldNode>());
    EXPECT_EQ(1, value);
}

TEST(WorldNode, Contains)
{
    auto sut = MAKE<WorldNode>();
    auto component = MAKE<WorldComponent<>>();
    auto component2 = MAKE<WorldComponent<>>();
    sut->Add(component);

    EXPECT_TRUE(sut->Contains(*component));
    EXPECT_FALSE(sut->Contains(*component2));
}

TEST(WorldNode, EnableType_IsTypeEnabled)
{
    auto sut = MAKE<WorldNode>();
    auto component = MAKE<TestComponent>();
    sut->Add(component);

    EXPECT_TRUE(sut->IsTypeEnabled<TestComponent>());
    sut->EnableType<TestComponent>(false);
    EXPECT_FALSE(sut->IsTypeEnabled<TestComponent>());
}
