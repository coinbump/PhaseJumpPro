#include "gtest/gtest.h"
#include "EaseFunc.h"
#include "SomeWorldComponent.h"
#include "TimeSlice.h"
#include "Vector2.h"
#include "WorldComponent.h"
#include "SomeKeyUIEvent.h"
#include "WorldNode.h"

using namespace PJ;
using namespace std;

namespace WorldComponentTests {
    class MarkerComponent : public WorldComponent {
    public:
        String TypeName() const override { return "MarkerComponent"; }
    };
}

using namespace WorldComponentTests;

TEST(WorldComponent, TestEnable)
{
    int changeCount{};
    
    WorldNode node;
    auto _sut = MAKE<WorldComponent>("");
    node.Add(_sut);

    auto& sut = *_sut;
    
    sut.attachmentCore.onEnabledChangeFunc = [&](auto& c) {
        changeCount++;
    };
    
    EXPECT_TRUE(sut.IsEnabled());
    sut.Enable(true);
    EXPECT_EQ(0, changeCount);
    sut.Enable(false);
    EXPECT_FALSE(sut.IsEnabled());
    EXPECT_EQ(1, changeCount);
}

TEST(WorldComponent, TestIsEnabledOwnerDestroyed)
{
    WorldNode node;
    auto _sut = MAKE<WorldComponent>("");
    node.Add(_sut);

    auto& sut = *_sut;
    
    EXPECT_TRUE(sut.IsEnabled());
    node.Destroy();
    EXPECT_FALSE(sut.IsEnabled());
}

TEST(SomeWorldComponent, LifeCycle)
{
    WorldComponent sut;
    EXPECT_FALSE(sut.IsAwake());
    EXPECT_FALSE(sut.IsStarted());
    
    sut.CheckedAwake();
    EXPECT_TRUE(sut.IsAwake());
    EXPECT_FALSE(sut.IsStarted());
    
    sut.CheckedStart();
    EXPECT_TRUE(sut.IsStarted());
}

TEST(SomeWorldComponent, Target)
{
    WorldNode node;
    WorldComponent sut;
    sut.targetFunc = [&](auto& owner) {
        return &node;
    };
    
    EXPECT_EQ(&node, sut.Target());
}

TEST(SomeWorldComponent, AddSignalIsListener)
{
    WorldNode node;
    auto sut = MAKE<WorldComponent>("");
    node.Add(sut);
    
    EXPECT_FALSE(node.IsListener());
    
    sut->AddSignalHandler({.id = "test"});
    
    EXPECT_TRUE(node.IsListener());
}

TEST(SomeWorldComponent, AddSignalTemplateIsListener)
{
    WorldNode node;
    auto sut = MAKE<WorldComponent>("");
    node.Add(sut);

    EXPECT_FALSE(node.IsListener());

    sut->AddSignalHandler<KeyDownUIEvent>({.id = "test"});

    EXPECT_TRUE(node.IsListener());
}

TEST(WorldComponent, IsDescendant_NoOwner_ReturnsFalse)
{
    WorldComponent sut;
    EXPECT_FALSE(sut.IsDescendant<MarkerComponent>());
}

TEST(WorldComponent, IsDescendant_NoParent_ReturnsFalse)
{
    auto root = MAKE<WorldNode>();
    auto _sut = MAKE<WorldComponent>("");
    root->Add(_sut);

    EXPECT_FALSE(_sut->IsDescendant<MarkerComponent>());
}

TEST(WorldComponent, IsDescendant_AncestorHasComponent_ReturnsTrue)
{
    auto root = MAKE<WorldNode>();
    root->AddComponent<MarkerComponent>();

    auto child = MAKE<WorldNode>();
    root->Add(child);

    auto grandchild = MAKE<WorldNode>();
    child->Add(grandchild);

    auto sut = MAKE<WorldComponent>("");
    grandchild->Add(sut);

    EXPECT_TRUE(sut->IsDescendant<MarkerComponent>());
}

TEST(WorldComponent, IsDescendant_OnlySelfHasComponent_ReturnsFalse)
{
    auto root = MAKE<WorldNode>();

    auto child = MAKE<WorldNode>();
    root->Add(child);
    child->AddComponent<MarkerComponent>();

    auto sut = MAKE<WorldComponent>("");
    child->Add(sut);

    EXPECT_FALSE(sut->IsDescendant<MarkerComponent>());
}

TEST(WorldComponent, IsDescendant_SiblingHasComponent_ReturnsFalse)
{
    auto root = MAKE<WorldNode>();

    auto child = MAKE<WorldNode>();
    root->Add(child);

    auto sibling = MAKE<WorldNode>();
    root->Add(sibling);
    sibling->AddComponent<MarkerComponent>();

    auto sut = MAKE<WorldComponent>("");
    child->Add(sut);

    EXPECT_FALSE(sut->IsDescendant<MarkerComponent>());
}

TEST(WorldComponent, Animate_AddsUpdatable)
{
    WorldComponent sut;

    EXPECT_EQ(0, sut.GetUpdatables().Count());

    float value{};
    sut.Animate<float>(
        { .duration = 1.0f,
          .startValue = 0.0f,
          .endValue = 100.0f,
          .setFunc = [&](float newValue) { value = newValue; } }
    );

    EXPECT_EQ(1, sut.GetUpdatables().Count());
}

TEST(WorldComponent, Animate_NoSetFunc_DoesNotAddUpdatable)
{
    WorldComponent sut;

    sut.Animate<float>({ .duration = 1.0f, .startValue = 0.0f, .endValue = 100.0f });

    EXPECT_EQ(0, sut.GetUpdatables().Count());
}

TEST(WorldComponent, Animate_DrivesBindingOverDuration)
{
    WorldComponent sut;

    float value{};
    sut.Animate<float>(
        { .duration = 1.0f,
          .easeFunc = EaseFuncs::linear,
          .startValue = 0.0f,
          .endValue = 100.0f,
          .setFunc = [&](float newValue) { value = newValue; } }
    );

    sut.OnUpdate(TimeSlice(0.5f));
    EXPECT_NEAR(50.0f, value, 0.0001f);

    sut.OnUpdate(TimeSlice(0.5f));
    EXPECT_NEAR(100.0f, value, 0.0001f);
}

TEST(WorldComponent, Animate_AppliesEaseFunc)
{
    WorldComponent sut;

    float linearValue{};
    sut.Animate<float>(
        { .duration = 1.0f,
          .easeFunc = EaseFuncs::linear,
          .startValue = 0.0f,
          .endValue = 100.0f,
          .setFunc = [&](float newValue) { linearValue = newValue; } }
    );

    float easedValue{};
    sut.Animate<float>(
        { .duration = 1.0f,
          .easeFunc = EaseFuncs::outCubed,
          .startValue = 0.0f,
          .endValue = 100.0f,
          .setFunc = [&](float newValue) { easedValue = newValue; } }
    );

    sut.OnUpdate(TimeSlice(0.5f));

    EXPECT_NEAR(50.0f, linearValue, 0.0001f);
    // outCubed is non-linear at t=0.5, so the eased value should differ from linear
    EXPECT_GT(easedValue, linearValue);
}

TEST(WorldComponent, Animate_Vector2_Interpolates)
{
    WorldComponent sut;

    Vector2 value;
    sut.Animate<Vector2>(
        { .duration = 1.0f,
          .easeFunc = EaseFuncs::linear,
          .startValue = Vector2(0, 0),
          .endValue = Vector2(100, 200),
          .setFunc = [&](Vector2 newValue) { value = newValue; } }
    );

    sut.OnUpdate(TimeSlice(0.5f));
    EXPECT_NEAR(50.0f, value.x, 0.0001f);
    EXPECT_NEAR(100.0f, value.y, 0.0001f);

    sut.OnUpdate(TimeSlice(0.5f));
    EXPECT_NEAR(100.0f, value.x, 0.0001f);
    EXPECT_NEAR(200.0f, value.y, 0.0001f);
}

TEST(WorldComponent, Name_SetName_RoundTrip)
{
    WorldComponent sut;
    sut.SetName("myName");
    EXPECT_EQ("myName", sut.Name());
}

TEST(WorldComponent, Name_EmptyFallsBackToTypeName)
{
    WorldComponent sut;
    // No SetName — Name() should fall back to TypeName()
    EXPECT_EQ(sut.TypeName(), sut.Name());
}

TEST(WorldComponent, Name_MarkerTypeNameFallback)
{
    MarkerComponent sut;
    EXPECT_EQ("MarkerComponent", sut.Name());
}

TEST(WorldComponent, Id_SetId_RoundTrip)
{
    WorldComponent sut;
    EXPECT_EQ("", sut.Id());
    sut.SetId("abc");
    EXPECT_EQ("abc", sut.Id());
}

TEST(WorldComponent, Target_NoTargetFunc_ReturnsOwner)
{
    WorldNode node;
    auto sut = MAKE<WorldComponent>("");
    node.Add(sut);

    EXPECT_EQ(&node, sut->Target());
}

TEST(WorldComponent, Signal_DispatchesToRegisteredHandler)
{
    WorldNode node;
    auto sut = MAKE<WorldComponent>("");
    node.Add(sut);

    int callCount{};
    sut->AddSignalHandler(
        { .id = "test.signal",
          .func = [&](auto& component, auto& signal) { callCount++; } }
    );

    sut->Signal("test.signal", Event<>{});
    EXPECT_EQ(1, callCount);

    sut->Signal("test.signal", Event<>{});
    EXPECT_EQ(2, callCount);
}

TEST(WorldComponent, Signal_UnregisteredIdDoesNotDispatch)
{
    WorldNode node;
    auto sut = MAKE<WorldComponent>("");
    node.Add(sut);

    int callCount{};
    sut->AddSignalHandler(
        { .id = "registered",
          .func = [&](auto& component, auto& signal) { callCount++; } }
    );

    sut->Signal("other", Event<>{});
    EXPECT_EQ(0, callCount);
}

TEST(WorldComponent, IsListenerToSignal_RegisteredId_ReturnsTrue)
{
    WorldNode node;
    auto sut = MAKE<WorldComponent>("");
    node.Add(sut);

    EXPECT_FALSE(sut->IsListenerToSignal("abc"));

    sut->AddSignalHandler({ .id = "abc" });

    EXPECT_TRUE(sut->IsListenerToSignal("abc"));
    EXPECT_FALSE(sut->IsListenerToSignal("xyz"));
}

TEST(WorldComponent, AwakeFunc_InvokedByCheckedAwake)
{
    WorldComponent sut;

    int awakeCount{};
    sut.awakeFunc = [&](auto&) { awakeCount++; };

    sut.CheckedAwake();
    EXPECT_EQ(1, awakeCount);

    // Second CheckedAwake should be a no-op — life tracks awake state
    sut.CheckedAwake();
    EXPECT_EQ(1, awakeCount);
}

TEST(WorldComponent, StartFunc_InvokedByCheckedStart)
{
    WorldComponent sut;

    int startCount{};
    sut.startFunc = [&](auto&) { startCount++; };

    // WorldPartLife::CheckedStart requires IsAwake() — otherwise it bails with an error log.
    sut.CheckedAwake();

    sut.CheckedStart();
    EXPECT_EQ(1, startCount);

    sut.CheckedStart();
    EXPECT_EQ(1, startCount);
}

TEST(WorldComponent, LateUpdateFunc_InvokedByLateUpdate)
{
    WorldComponent sut;

    int count{};
    sut.lateUpdateFunc = [&](auto&) { count++; };

    sut.LateUpdate();
    sut.LateUpdate();
    EXPECT_EQ(2, count);
}
