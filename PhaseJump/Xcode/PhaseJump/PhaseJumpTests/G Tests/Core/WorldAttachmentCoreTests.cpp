#include "gtest/gtest.h"
#include "WorldAttachmentCore.h"
#include "Utils.h"
#include "WorldComponent.h"
#include "SomeSignal.h"

using namespace PJ;
using namespace std;

namespace WorldAttachmentCoreTests {
    class TestSignal : public SomeSignal {
        
    };
}

using namespace WorldAttachmentCoreTests;

TEST(WorldAttachmentCore, OnEnabledChanged) {
    WorldComponent component;
    WorldAttachmentCore<SomeWorldComponent> sut(component);

    int count{};
    sut.onEnabledChangeFunc = [&](auto& core) {
        count++;
    };
    
    ASSERT_EQ(0, count);
    ASSERT_TRUE(sut.IsEnabled());
    
    sut.Enable(true);
    ASSERT_EQ(0, count);
    ASSERT_TRUE(sut.IsEnabled());
    
    sut.Enable(false);
    ASSERT_EQ(1, count);
    ASSERT_FALSE(sut.IsEnabled());
}

TEST(WorldAttachmentCore, OnAddSignalHandler) {
    WorldComponent component;
    WorldAttachmentCore<SomeWorldComponent> sut(component);

    int count{};
    sut.onAddSignalHandlerFunc = [&](auto& core, auto& signalHandler) {
        count++;
    };
    
    ASSERT_EQ(0, count);
    
    sut.AddSignalHandler({.id = "test", .func = [&](auto& component, auto& signal) {
    }});
    
    ASSERT_EQ(1, count);
}

TEST(WorldAttachmentCore, Signal) {
    WorldComponent component;
    WorldAttachmentCore<SomeWorldComponent> sut(component);

    int signalCount{};
    
    sut.AddSignalHandler({.id = "test", .func = [&](auto& component, auto& signal) {
        signalCount++;
    }});
    
    Event event;
    sut.Signal("test", event);
    
    ASSERT_EQ(1, signalCount);
}

TEST(WorldAttachmentCore, SignalTypeSafe) {
    WorldComponent component;
    WorldAttachmentCore<SomeWorldComponent> sut(component);

    int signalCount{};
    
    sut.AddSignalHandler<Event<>>(
        { .id = "test",
          .func = [&](auto& component, auto& event) {
        signalCount++;
    }});
    
    Event event;
    sut.Signal("test", event);
    
    ASSERT_EQ(1, signalCount);
    
    TestSignal testSignal;
    sut.Signal("test", testSignal);
    
    ASSERT_EQ(1, signalCount);
}

TEST(WorldAttachmentCore, SignalMultiple)
{
    WorldComponent component;
    WorldAttachmentCore<SomeWorldComponent> sut(component);

    int signalCount{};

    auto add = [&]() {
        sut.AddSignalHandler({.id = "test", .func = [&](auto& component, auto& signal) {
            signalCount++;
        }});
    };
    
    add();
    add();
    
    Event event;
    sut.Signal("test", event);
    
    ASSERT_EQ(2, signalCount);
}

TEST(WorldAttachmentCore, Updatable)
{
    WorldComponent component;
    WorldAttachmentCore<SomeWorldComponent> sut(component);
    
    float timeValue{};
    
    sut.updatable.onUpdateFunc = [&](auto& updatable, auto time) {
        timeValue += time.delta;
        return FinishType::Continue;
    };
    
    sut.OnUpdate({3});
    EXPECT_EQ(3.0f, timeValue);
}

TEST(WorldAttachmentCore, Updatables)
{
    WorldComponent component;
    WorldAttachmentCore<SomeWorldComponent> sut(component);
    
    float timeValue{};
    
    sut.updatables.Add([&](auto time) {
        timeValue += time.delta;
        return FinishType::Continue;
    });
    
    sut.OnUpdate({3});
    EXPECT_EQ(3.0f, timeValue);
}
