#include "gtest/gtest.h"
#include "View2D.h"
#include "World.h"
#include "QuickBuild.h"
#include "WorldNode.h"
#include "PadViewLayout.h"
#include "ViewBuilder.h"

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
    
    EXPECT_EQ(Vector2(0, 0), sut.ToViewPosition(LayoutAnchor2D::topLeft));
    EXPECT_EQ(Vector2(5, 5), sut.ToViewPosition(LayoutAnchor2D::center));
    EXPECT_EQ(Vector2(10, 10), sut.ToViewPosition(LayoutAnchor2D::bottomRight));
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

TEST(View2D, TopLeftLocalPosition) {
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
    Rect frame{ .origin = {10, 10}, .size = {20, 20}};
    view->SetFrame(frame);
    
    EXPECT_EQ(Vector3(50 * vecLeft, 50 * vecUp, 0), parentView->TopLeftLocalPosition());
    EXPECT_EQ(Vector3(10 * vecLeft, 10 * vecUp, 0), view->TopLeftLocalPosition());
}

TEST(View2D, ChildTopLeftLocalPosition) {
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
    Rect frame{ .origin = {10, 10}, .size = {20, 20}};
    view->SetFrame(frame);
    
    Vector3 expectedValue(50 * vecLeft + 10 * vecRight, 50 * vecUp + 10 * vecDown, 0);
    EXPECT_EQ(expectedValue, parentView->ChildTopLeftLocalPosition(frame));
    EXPECT_EQ(Vector3(), view->ChildTopLeftLocalPosition(frame));
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
    
    parentView->SetFrameSize({100, 100});
    Rect frame{ .origin = {10, 10}, .size = {20, 20}};
    view->SetFrame(frame);
    
    Vector3 expectedValue(50 * vecLeft + 20 * vecRight, 50 * vecUp + 20 * vecDown, 0);
    EXPECT_EQ(expectedValue, parentView->ChildLocalPosition(frame));
}

TEST(View2D, ViewToWorld) {
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

    EXPECT_EQ(Vector3(50 * vecLeft, 50 * vecUp, 0), parentView->ViewToWorld({0, 0}));
    EXPECT_EQ(Vector3(0, 0, 0), parentView->ViewToWorld({50, 50}));
    EXPECT_EQ(Vector3(50 * vecRight, 50 * vecDown, 0), parentView->ViewToWorld({100, 100}));
    
    Vector3 expectedValue(50 * vecLeft + 10 * vecRight, 50 * vecUp + 10 * vecDown, 0);
    EXPECT_EQ(expectedValue, view->ViewToWorld({0, 0}));
    EXPECT_EQ(Vector2(0, 0), view->WorldToView(expectedValue));
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

TEST(View2D, LocalToView) {
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
    Rect frame{ .origin = {10, 10}, .size = {20, 20}};
    view->SetFrame(frame);

    EXPECT_EQ(Vector2(frame.size.x/2.0f, frame.size.y/2.0f), view->LocalToView({0, 0, 0}));
    EXPECT_EQ(Vector2(0, 0), view->LocalToView({-10, 10, 3}));
    
    EXPECT_EQ(Vector3(0, 0, 0), view->ViewToLocal({frame.size.x/2.0f, frame.size.y/2.0f}));
    EXPECT_EQ(Vector3(-10, 10, 0), view->ViewToLocal({0, 0}));
}

TEST(View2D, ViewToLocal) {
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
    
    EXPECT_EQ(Vector3(0, 0, 0), parentView->ViewToLocal({50, 50}));
    EXPECT_EQ(Vector3(10 * vecLeft, 10 * vecUp, 0), parentView->ViewToLocal({40, 40}));
    EXPECT_EQ(Vector3(10 * vecRight, 10 * vecDown, 0), parentView->ViewToLocal({60, 60}));
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

TEST(View2D, TopLevelViews) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    world.Add(parentNode);
    
    View2D* childView{};
    View2D* topLevelView{};
    VectorList<View2D*> views;
    
    ViewBuilder vb(*parentNode);
    vb.RootView({100, 100}, [&](auto& vb) {
        vb.HStack({VIEW(&) {
            childView = vb.ActiveView();
            views.push_back(vb.ActiveView());
        }});
        vb.HStack({VIEW(&) {
            topLevelView = vb.ActiveView();
            views.push_back(vb.ActiveView());
        }});
    });
    
    ASSERT_EQ(2, views.size());
    ASSERT_NE(nullptr, childView);
    ASSERT_NE(nullptr, topLevelView);
    
    EXPECT_EQ(topLevelView, views[1]);
    
    EXPECT_FALSE(childView->IsTopLevel());
    
    auto topLevelViews = childView->TopLevelViews();
    ASSERT_EQ(1, topLevelViews.size());
    EXPECT_EQ(topLevelView, topLevelViews[0]);
}

TEST(View2D, Present) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    world.Add(parentNode);
    
    ViewBuilder vb(*parentNode);
    vb.RootView({100, 100}, [&](auto& vb) {
        vb.Color(Color::blue);
    });
    
    View2D* topLevelView{};
    
    auto view = vb.ActiveView();
    Rect frame{.origin = {10, 10}, .size = {20, 20}};
    view->Present({.makeFrameFunc = [&](auto& presentingView, auto& view) {
        return frame;
    },
        VIEW(&) {
            vb
                .Color(Color::red).
            QB().template ModifyLatest<View2D>([&](auto& view) {
                topLevelView = &view;
            });
        }
    });
    
    ASSERT_NE(nullptr, topLevelView);
    
    auto topLevelViews = view->TopLevelViews();
    ASSERT_EQ(1, topLevelViews.size());
    EXPECT_EQ(topLevelView, topLevelViews[0]);
    EXPECT_TRUE(topLevelView->IsTopLevel());
}

TEST(View2D, Dismiss) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    world.Add(parentNode);
    
    ViewBuilder vb(*parentNode);
    vb.RootView({100, 100}, [&](auto& vb) {
        vb.Color(Color::blue);
    });
    auto rootView = vb.ActiveView();
    ASSERT_NE(nullptr, rootView);
    
    ASSERT_EQ(1, rootView->ChildViews().size());
    
    View2D* topLevelView{};
    
    Rect frame{.origin = {10, 10}, .size = {20, 20}};
    rootView->Present({.makeFrameFunc = [&](auto& presentingView, auto& view) {
        return frame;
    },
        VIEW(&) {
            vb
                .Color(Color::red).
            QB().template ModifyLatest<View2D>([&](auto& view) {
                topLevelView = &view;
            });
        }
    });
    
    ASSERT_EQ(2, rootView->ChildViews().size());
    
    ASSERT_NE(nullptr, topLevelView);
    rootView->Dismiss(*topLevelView);
    
    auto topLevelViews = rootView->TopLevelViews();
    ASSERT_EQ(0, topLevelViews.size());
    
    // Make sure the node has been removed for dismiss, not just the view
    ASSERT_EQ(1, rootView->ChildViews().size());
}

TEST(View2D, PopTopLevel) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    world.Add(parentNode);
    
    ViewBuilder vb(*parentNode);
    vb.RootView({100, 100}, [&](auto& vb) {
        vb.Color(Color::blue);
    });
    
    View2D* topLevelView{};
    
    auto view = vb.ActiveView();
    Rect frame{.origin = {10, 10}, .size = {20, 20}};
    view->Present({.makeFrameFunc = [&](auto& presentingView, auto& view) {
        return frame;
    },
        VIEW(&) {
            vb
                .Color(Color::red).
            QB().template ModifyLatest<View2D>([&](auto& view) {
                topLevelView = &view;
            });
        }
    });
    
    ASSERT_NE(nullptr, topLevelView);
    view->PopTopLevel();
    
    auto topLevelViews = view->TopLevelViews();
    ASSERT_EQ(0, topLevelViews.size());
}

TEST(View2D, PopTo) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    world.Add(parentNode);
    
    ViewBuilder vb(*parentNode);
    vb.RootView({100, 100}, [&](auto& vb) {
        vb.Color(Color::blue);
    });
    
    View2D* topLevelView{};
    
    auto view = vb.ActiveView();
    Rect frame{.origin = {10, 10}, .size = {20, 20}};
    view->Present({.makeFrameFunc = [&](auto& presentingView, auto& view) {
        return frame;
    },
        VIEW(&) {
            vb
                .Color(Color::red).
            QB().template ModifyLatest<View2D>([&](auto& view) {
                topLevelView = &view;
            });
        }
    });
    
    ASSERT_NE(nullptr, topLevelView);
    view->PopTo(*topLevelView);
    
    auto topLevelViews = view->TopLevelViews();
    ASSERT_EQ(1, topLevelViews.size());
    
    view->PopTo(*view);
    
    topLevelViews = view->TopLevelViews();
    ASSERT_EQ(0, topLevelViews.size());
}
