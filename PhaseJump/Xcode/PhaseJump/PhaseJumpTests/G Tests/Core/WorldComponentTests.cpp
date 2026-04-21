#include "gtest/gtest.h"
#include "SomeWorldComponent.h"
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
