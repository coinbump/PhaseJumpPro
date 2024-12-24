#include "gtest/gtest.h"
#include "SomeWorldSystem.h"
#include "SomeKeyUIEvent.h"

using namespace PJ;
using namespace std;

namespace SomeWorldSystemTests {
}

using namespace SomeWorldSystemTests;

TEST(SomeWorldSystem, LifeCycle)
{
    SomeWorldSystem sut;
    EXPECT_FALSE(sut.IsAwake());
    EXPECT_FALSE(sut.IsStarted());
    
    sut.CheckedAwake();
    EXPECT_TRUE(sut.IsAwake());
    EXPECT_FALSE(sut.IsStarted());
    
    sut.CheckedStart();
    EXPECT_TRUE(sut.IsStarted());
}

TEST(SomeWorldSystem, AddKeyFunc)
{
    int count{};
    
    SomeWorldSystem sut;
    sut.AddKeyFunc(10, {KeyModifier::Shift, KeyModifier::Control}, [&](auto& system) {
        count++;
    });
        
    KeyDownUIEvent eventMatch(0, 10, {.modifiers = {KeyModifier::Shift, KeyModifier::Control}});
    sut.Signal(SignalId::KeyDown, eventMatch);
    
    EXPECT_EQ(1, count);
    
    KeyDownUIEvent eventWrongKeyCode(0, 9, {.modifiers = {KeyModifier::Shift, KeyModifier::Control}});
    sut.Signal(SignalId::KeyDown, eventWrongKeyCode);
    
    EXPECT_EQ(1, count);
    
    KeyDownUIEvent eventWrongModifiers(0, 10, {.modifiers = {KeyModifier::Shift}});
    sut.Signal(SignalId::KeyDown, eventWrongModifiers);
    
    EXPECT_EQ(1, count);
}
