#include "gtest/gtest.h"
#include "SomeWorldComponent.h"
#include "WorldComponent.h"
#include "SomeKeyUIEvent.h"
#include "WorldNode.h"

using namespace PJ;
using namespace std;

namespace SomeWorldComponentTests {
}

using namespace SomeWorldComponentTests;

TEST(WorldComponent, TestEnable)
{
    int changeCount{};
    
    WorldNode node;
    auto _sut = MAKE<WorldComponent<>>("");
    node.Add(_sut);

    auto& sut = *_sut;
    
    sut.onEnabledChangeFunc = [&](auto& c) {
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
    auto _sut = MAKE<WorldComponent<>>("");
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
    auto sut = MAKE<WorldComponent<>>("");
    node.Add(sut);
    
    EXPECT_FALSE(node.isListener);
    
    sut->AddSignalHandler({.id = "test"});
    
    EXPECT_TRUE(node.isListener);
}

TEST(SomeWorldComponent, AddSignalTemplateIsListener)
{
    WorldNode node;
    auto sut = MAKE<WorldComponent<>>("");
    node.Add(sut);
    
    EXPECT_FALSE(node.isListener);
    
    sut->AddSignalHandler<KeyDownUIEvent>({.id = "test"});
    
    EXPECT_TRUE(node.isListener);
}
