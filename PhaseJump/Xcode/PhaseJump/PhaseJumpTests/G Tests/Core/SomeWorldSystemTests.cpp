#include "gtest/gtest.h"
#include "SomeWorldSystem.h"
#include "SomeKeyUIEvent.h"

using namespace PJ;
using namespace std;

namespace WorldSystemTests {
}

using namespace WorldSystemTests;

TEST(WorldSystem, LifeCycle)
{
    WorldSystem sut;
    EXPECT_FALSE(sut.IsAwake());
    EXPECT_FALSE(sut.IsStarted());
    
    sut.CheckedAwake();
    EXPECT_TRUE(sut.IsAwake());
    EXPECT_FALSE(sut.IsStarted());
    
    sut.CheckedStart();
    EXPECT_TRUE(sut.IsStarted());
}

TEST(WorldSystem, AddKeyFunc)
{
    int count{};
    
    WorldSystem sut;
    sut.AddKeyFunc(10, {KeyModifier::Shift, KeyModifier::Control}, [&](auto& system) {
        count++;
    });
        
    KeyDownUIEvent eventMatch(KeyUIEventCore{.scanCode = 0, .keyCode = 10, .keyModifiers = {.modifiers = {KeyModifier::Shift, KeyModifier::Control}}});
    sut.Signal(SignalId::KeyDown, eventMatch);
    
    EXPECT_EQ(1, count);
    
    KeyDownUIEvent eventWrongKeyCode(KeyUIEventCore{.scanCode = 0, .keyCode = 9, .keyModifiers = {.modifiers = {KeyModifier::Shift, KeyModifier::Control}}});
    sut.Signal(SignalId::KeyDown, eventWrongKeyCode);
    
    EXPECT_EQ(1, count);
    
    KeyDownUIEvent eventWrongModifiers(KeyUIEventCore{.scanCode = 0, .keyCode = 10, .keyModifiers = {.modifiers = {KeyModifier::Shift}}});
    sut.Signal(SignalId::KeyDown, eventWrongModifiers);
    
    EXPECT_EQ(1, count);
}
