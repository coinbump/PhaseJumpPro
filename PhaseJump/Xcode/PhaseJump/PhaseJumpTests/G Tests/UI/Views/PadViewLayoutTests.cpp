#include "gtest/gtest.h"
#include "PadViewLayout.h"
#include "View2D.h"
#include "World.h"
#include "WorldNode.h"
#include "ColorView.h"

using namespace std;
using namespace PJ;

namespace PadViewLayoutTests {
}

using namespace PadViewLayoutTests;

TEST(PadViewLayout, Test) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto padNode = MAKE<WorldNode>();
    auto padView = MAKE<View2D>();
    
    parentNode->Add(padNode);
    padNode->Add(padView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    padNode->Add(node);
    node->Add(view);
    
    parentView->SetFrameSize({100, 100});
    view->SetFixedSize(10, 10);
    
    UP<SomeViewLayout> layout = NEW<PadViewLayout>(LayoutInsets{.leading = 4, .trailing = 10, .top = 2, .bottom = 4});
    padView->SetLayout(layout);
    padView->LayoutIfNeeded();
    
    EXPECT_EQ(Vector3(0, 0, 0), parentView->owner->transform.WorldPosition());
    EXPECT_EQ(Vector2(38, 42), padView->Frame().origin);
    EXPECT_EQ(Vector2(10 + 14, 10 + 6), padView->Frame().size);

    EXPECT_EQ(Vector2(4, 2), view->Frame().origin);
}

TEST(PadViewLayout, Unbounded) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto padNode = MAKE<WorldNode>();
    auto padView = MAKE<View2D>();
    
    parentNode->Add(padNode);
    padNode->Add(padView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();
    view->SetFixedSize(FloatMath::maxValue, FloatMath::maxValue);
    
    padNode->Add(node);
    node->Add(view);
    
    parentView->SetFrameSize({100, 100});
    
    UP<SomeViewLayout> layout = NEW<PadViewLayout>(LayoutInsets{.leading = 4, .trailing = 10, .top = 2, .bottom = 4});
    padView->SetLayout(layout);
    padView->LayoutIfNeeded();
    
    EXPECT_EQ(Vector3(0, 0, 0), parentView->owner->transform.WorldPosition());
    EXPECT_EQ(Vector2(0, 0), padView->Frame().origin);
    EXPECT_EQ(Vector2(100, 100), padView->Frame().size);

    EXPECT_EQ(Vector2(4, 2), view->Frame().origin);
    EXPECT_EQ(Vector2(100 - 14, 100 - 6), view->Frame().size);
}
