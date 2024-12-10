#include "gtest/gtest.h"
#include "ZStackViewLayout.h"
#include "View2D.h"
#include "World.h"
#include "WorldNode.h"
#include "SpacerView.h"

using namespace std;
using namespace PJ;

namespace ZStackViewLayoutTests {
}

using namespace ZStackViewLayoutTests;

TEST(ZStackViewLayout, AllFixedSize) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto stackNode = MAKE<WorldNode>();
    auto stackView = MAKE<View2D>();
    
    parentNode->Add(stackNode);
    stackNode->Add(stackView);
    
    VectorList<View2D*> views;
    
    for (int i = 0; i < 2; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());
        
        stackNode->Add(node);
        node->Add(view);
        
        view->SetFixedSize((float)(i + 1)*10, (float)(i + 1)*10);
    }
    
    parentView->SetFrameSize({100, 100});
    
    UP<SomeViewLayout> layout = NEW<ZStackViewLayout>();
    stackView->SetLayout(layout);
    stackView->LayoutIfNeeded();

    EXPECT_EQ(Vector2(20, 20), stackView->Frame().size);
    EXPECT_EQ(Vector2(5, 5), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(10, 10), views[0]->Frame().size);
    EXPECT_EQ(Vector2(0, 0), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
}

TEST(ZStackViewLayout, MixedFixedSize) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto stackNode = MAKE<WorldNode>();
    auto stackView = MAKE<View2D>();
    
    parentNode->Add(stackNode);
    stackNode->Add(stackView);
    
    VectorList<View2D*> views;
    
    for (int i = 0; i < 3; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());
        
        stackNode->Add(node);
        node->Add(view);
        
        if (i == 1) {
            view->SetFixedSize((float)(i + 1)*10, (float)(i + 1)*10);
        } else {
            view->SetFixedSize(FloatMath::maxValue, FloatMath::maxValue);
        }
    }
    
    parentView->SetFrameSize({100, 100});
    
    UP<SomeViewLayout> layout = NEW<ZStackViewLayout>();
    stackView->SetLayout(layout);
    stackView->LayoutIfNeeded();

    EXPECT_EQ(Vector2(100, 100), stackView->Frame().size);
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(100, 100), views[0]->Frame().size);
    EXPECT_EQ(Vector2(40, 40), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
    EXPECT_EQ(Vector2(0, 0), views[2]->Frame().origin);
    EXPECT_EQ(Vector2(100, 100), views[2]->Frame().size);
}
