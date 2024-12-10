#include "gtest/gtest.h"
#include "View2D.h"
#include "World.h"
#include "WorldNode.h"
#include "PadViewLayout.h"

using namespace std;
using namespace PJ;

namespace View2DTests {
}

using namespace View2DTests;

TEST(View2D, LayoutIfNeeded) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    EXPECT_TRUE(parentView->NeedsLayout());
    parentView->CheckedAwake();
    parentView->CheckedStart();
    EXPECT_FALSE(parentView->NeedsLayout());
    
    parentView->SetFrameSize({100, 100});
    EXPECT_TRUE(parentView->NeedsLayout());
    parentView->LayoutIfNeeded();
    EXPECT_FALSE(parentView->NeedsLayout());
    
    view->SetFrameSize({20, 20});
    EXPECT_TRUE(parentView->NeedsLayout());
    view->LayoutIfNeeded();
    EXPECT_FALSE(parentView->NeedsLayout());
}

TEST(View2D, ParentBounds) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    parentView->SetFrameSize({100, 100});
    EXPECT_EQ(std::nullopt, parentView->ParentBounds());
   
    Rect expectedValue{.origin = {0, 0}, .size = {100, 100}};
    EXPECT_EQ(expectedValue, view->ParentBounds());
}

TEST(View2D, ParentView) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    EXPECT_EQ(parentView.get(), view->ParentView());
    EXPECT_EQ(nullptr, parentView->ParentView());
}

TEST(View2D, RootView) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    EXPECT_EQ(parentView.get(), view->RootView());
    EXPECT_EQ(parentView.get(), parentView->RootView());
}

TEST(View2D, IsRoot) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    EXPECT_TRUE(parentView->IsRoot());
    EXPECT_FALSE(view->IsRoot());
}

TEST(View2D, Bounds) {
    auto sut = MAKE<View2D>();
    sut->SetFrame({.origin = {10, 10}, .size = {5, 5}});
    
    Rect expectedValue{.origin = {0, 0}, .size = {5, 5}};
    EXPECT_EQ(expectedValue, sut->Bounds());
}

TEST(View2D, FirstChildView) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    auto node2 = MAKE<WorldNode>();
    auto view2 = MAKE<View2D>();

    parentNode->Add(node2);
    node2->Add(view2);
    
    EXPECT_EQ(view.get(), parentView->FirstChildView());
}

TEST(View2D, ChildViews) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    auto node2 = MAKE<WorldNode>();
    auto view2 = MAKE<View2D>();

    parentNode->Add(node2);
    node2->Add(view2);
    
    VectorList<View2D*> expectedValue{view.get(), view2.get()};
    EXPECT_EQ(expectedValue, parentView->ChildViews());
}

TEST(View2D, ToViewPosition) {
    View2D sut;
    sut.SetFrame({.origin = {10, 10}, .size = {10, 10}});
    
    EXPECT_EQ(Vector2(0, 0), sut.ToViewPosition(ViewPoint::topLeading));
    EXPECT_EQ(Vector2(5, 5), sut.ToViewPosition(ViewPoint::center));
    EXPECT_EQ(Vector2(10, 10), sut.ToViewPosition(ViewPoint::bottomTrailing));
}

TEST(View2D, TopLeftWorldPosition) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    EXPECT_TRUE(parentView->NeedsLayout());
    parentView->CheckedAwake();
    parentView->CheckedStart();
    EXPECT_FALSE(parentView->NeedsLayout());
    
    parentView->SetFrameSize({100, 100});
    Rect frame{ .origin = {10, 10}, .size = {20, 20}};
    view->SetFrame(frame);
    
    // Layout must be applied for transform funcs
    node->SetLocalPosition({-50 + (frame.origin.x + frame.size.x / 2.0f) * vecRight, 50 + (frame.origin.y + frame.size.y / 2.0f) * vecDown, 0});

    Vector3 expectedValue(-50 + 10 * vecRight, 50 + 10 * vecDown, 0);
    EXPECT_EQ(expectedValue, view->TopLeftWorldPosition());
}

TEST(View2D, ViewToWorldPosition) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    EXPECT_TRUE(parentView->NeedsLayout());
    parentView->CheckedAwake();
    parentView->CheckedStart();
    EXPECT_FALSE(parentView->NeedsLayout());
    
    parentView->SetFrameSize({100, 100});
    Rect frame{ .origin = {10, 10}, .size = {20, 20}};
    view->SetFrame(frame);
    
    // Layout must be applied for transform funcs
    node->SetLocalPosition({-50 + (frame.origin.x + frame.size.x / 2.0f) * vecRight, 50 + (frame.origin.y + frame.size.y / 2.0f) * vecDown, 0});

    EXPECT_EQ(Vector3(-50, 50, 0), parentView->ViewToWorldPosition({0, 0}));
    EXPECT_EQ(Vector3(0, 0, 0), parentView->ViewToWorldPosition({50, 50}));
    EXPECT_EQ(Vector3(50, -50, 0), parentView->ViewToWorldPosition({100, 100}));
    
    Vector3 expectedValue(-50 + 10 * vecRight, 50 + 10 * vecDown, 0);
    EXPECT_EQ(expectedValue, view->ViewToWorldPosition({0, 0}));
    EXPECT_EQ(Vector2(0, 0), view->WorldToViewPosition(expectedValue));
}

TEST(View2D, TestViewPositionHit) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    EXPECT_TRUE(parentView->NeedsLayout());
    parentView->CheckedAwake();
    parentView->CheckedStart();
    EXPECT_FALSE(parentView->NeedsLayout());
    
    parentView->SetFrameSize({100, 100});
    Rect frame{ .origin = {10, 10}, .size = {20, 20}};
    view->SetFrame(frame);
    
    EXPECT_TRUE(view->TestViewPositionHit({0, 0}));
    EXPECT_TRUE(view->TestViewPositionHit({19.5f, 19.5f}));
    EXPECT_FALSE(view->TestViewPositionHit({-1, 0}));
    EXPECT_FALSE(view->TestViewPositionHit({21, 5}));
}

TEST(View2D, LocalToViewPosition) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    EXPECT_TRUE(parentView->NeedsLayout());
    parentView->CheckedAwake();
    parentView->CheckedStart();
    EXPECT_FALSE(parentView->NeedsLayout());
    
    Rect frame{ .origin = {10, 10}, .size = {20, 20}};
    view->SetFrame(frame);

    EXPECT_EQ(Vector2(frame.size.x/2.0f, frame.size.y/2.0f), view->LocalToViewPosition({0, 0, 0}));
    EXPECT_EQ(Vector2(0, 0), view->LocalToViewPosition({-10, 10, 3}));
}

TEST(View2D, ChildLocalPosition) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    EXPECT_TRUE(parentView->NeedsLayout());
    parentView->CheckedAwake();
    parentView->CheckedStart();
    EXPECT_FALSE(parentView->NeedsLayout());
    
    parentView->SetFrameSize({100, 100});
    Rect frame{ .origin = {10, 10}, .size = {20, 20}};
    view->SetFrame(frame);
    
    Vector3 expectedValue{-50 + (frame.origin.x + frame.size.x / 2.0f) * vecRight, 50 + (frame.origin.y + frame.size.y / 2.0f) * vecDown, 0};
    EXPECT_EQ(expectedValue, parentView->ChildLocalPosition(view->Frame()));
}

TEST(View2D, TestSetLayout) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    parentView->SetFrameSize({100, 100});
    parentView->LayoutIfNeeded();
    
    EXPECT_FALSE(view->NeedsLayout());
    
    UP<SomeViewLayout> layout = NEW<PadViewLayout>(LayoutInsets{.leading = 5, .trailing = 10, .top = 2, .bottom = 4});
    view->SetLayout(layout);
    
    EXPECT_TRUE(view->NeedsLayout());
    view->OnUpdate({0});
    EXPECT_FALSE(view->NeedsLayout());
}

TEST(View2D, ApplyFixedSizeSizeConstraints) {
    View2D sut;
    sut.SetFixedSize(10, 20);
    
    EXPECT_EQ(Vector2(10, 20), sut.WithSizeConstraints(Vector2()));
}

TEST(View2D, ApplyMaxSizeConstraints) {
    View2D sut;
    sut.SetMaxSize(10, 20);
    
    EXPECT_EQ(Vector2(5, 20), sut.WithSizeConstraints(Vector2(5, 300)));
}

TEST(View2D, EnvironmentValue) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    View2D *_view{}, *_view2{};
    
    parentView->buildViewFunc = [&](auto& parentView) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();

        parentView.owner->Add(node);
        node->Add(view);
        _view = view.get();
        
        auto node2 = MAKE<WorldNode>();
        auto view2 = MAKE<View2D>();

        node->Add(node2);
        node2->Add(view2);
        _view2 = view2.get();
    };
    
    EXPECT_EQ(nullptr, _view);
    EXPECT_EQ(nullptr, _view2);
    
    parentView->SetEnvironmentValue<int>("int", 10);
    parentView->RebuildIfNeeded();
    
    ASSERT_NE(nullptr, _view);
    ASSERT_NE(nullptr, _view2);
    EXPECT_EQ(10, _view2->EnvironmentValue<int>("int"));
    EXPECT_EQ(10, _view->EnvironmentValue<int>("int"));
    EXPECT_EQ(10, parentView->EnvironmentValue<int>("int"));
    
    _view->SetEnvironmentValue("int", 5);
    EXPECT_EQ(5, _view2->EnvironmentValue<int>("int"));
    EXPECT_EQ(5, _view->EnvironmentValue<int>("int"));
    EXPECT_EQ(10, parentView->EnvironmentValue<int>("int"));
}
