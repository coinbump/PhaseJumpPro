#include "gtest/gtest.h"
#include "ViewProxy.h"
#include "World.h"
#include "WorldNode.h"
#include "View2D.h"

using namespace std;
using namespace PJ;

namespace ViewProxyTests {
    class TestBoundedView: public View2D {
    public:
        using Base = View2D;
        
        // MARK: View2D
        
        ViewProxy MakeViewProxy() override {
            auto result = Base::MakeViewProxy();
            
            result.viewSizeFunc = [](auto& proxy, auto proposal) {
                return Vector2{10, 10};
            };
            
            return result;
        }
    };
    
    class TestUnboundedView: public View2D {
    public:
        using Base = View2D;
        
        // MARK: View2D
        
        ViewProxy MakeViewProxy() override {
            auto result = Base::MakeViewProxy();
            
            result.viewSizeFunc = [](auto& proxy, auto proposal) {
                return Vector2::max;
            };
            
            return result;
        }
    };
}

using namespace ViewProxyTests;

TEST(ViewProxy, MakePlaceViewFuncTopLeadingSameSizeParent) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    parentView->SetFrameSize({20, 10});
    
    auto sut = ViewProxy::MakePlaceViewFunc(*view);
    
    ViewProxy proxy({});
    proxy.viewSizeFunc = ViewProxy::MakeFixedViewSizeFunc({20, 10});
    
    ViewPoint viewPoint = ViewPoint::topLeading;
    ViewSizeProposal proposal{100, 100};
    
    sut(proxy, {0, 0}, viewPoint, proposal);
    
    auto localPos = view->owner->transform.LocalPosition();
    EXPECT_EQ(Vector3(0, 0, 0), localPos);
    EXPECT_EQ(Vector2(0, 0), view->Frame().origin);
}

TEST(ViewProxy, MakePlaceViewFuncCenterSameSizeParent) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    parentView->SetFrameSize({20, 10});
    
    auto sut = ViewProxy::MakePlaceViewFunc(*view);
    
    ViewProxy proxy({});
    proxy.viewSizeFunc = ViewProxy::MakeFixedViewSizeFunc({20, 10});
    
    ViewPoint viewPoint = ViewPoint::center;
    ViewSizeProposal proposal{100, 100};
    
    sut(proxy, {0, 0}, viewPoint, proposal);
    
    auto localPos = view->owner->transform.LocalPosition();
    EXPECT_EQ(Vector3(-10, 5, 0), localPos);
    EXPECT_EQ(Vector2(-10, -5), view->Frame().origin);
}

TEST(ViewProxy, MakePlaceViewFuncCenterSameSizeParentOffset) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);
    
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();

    parentNode->Add(node);
    node->Add(view);
    
    parentView->SetFrameSize({20, 10});
    
    auto sut = ViewProxy::MakePlaceViewFunc(*view);
    
    ViewProxy proxy({});
    proxy.viewSizeFunc = ViewProxy::MakeFixedViewSizeFunc({20, 10});
    
    ViewPoint viewPoint = ViewPoint::center;
    ViewSizeProposal proposal{100, 100};
    
    sut(proxy, {4, 3}, viewPoint, proposal);
    
    auto localPos = view->owner->transform.LocalPosition();
    EXPECT_EQ(Vector3(-10 + 4 * vecRight, 5 + 3 * vecDown, 0), localPos);
    EXPECT_EQ(Vector2(-10 + 4, -5 + 3), view->Frame().origin);
}

TEST(ViewProxy, MakePlaceViewFuncTopLeadingBigParent) {
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
    
    auto sut = ViewProxy::MakePlaceViewFunc(*view);
    
    ViewProxy proxy({});
    proxy.viewSizeFunc = ViewProxy::MakeFixedViewSizeFunc({20, 10});
    
    ViewPoint viewPoint = ViewPoint::topLeading;
    ViewSizeProposal proposal{100, 100};
    
    sut(proxy, {0, 0}, viewPoint, proposal);
    
    auto localPos = view->owner->transform.LocalPosition();
    EXPECT_EQ(Vector3(-50 + 10 * vecRight, 50 + 5 * vecDown, 0), localPos);
    EXPECT_EQ(Vector2(0, 0), view->Frame().origin);
}

TEST(ViewProxy, IsUnbounded) {
    TestUnboundedView sut;
    
    auto proxy = sut.MakeViewProxy();
    EXPECT_TRUE(proxy.IsWidthUnbounded(ViewSizeProposal::ideal));
    EXPECT_TRUE(proxy.IsHeightUnbounded(ViewSizeProposal::ideal));
    EXPECT_TRUE(proxy.IsAxisUnbounded(Axis2D::X, ViewSizeProposal::ideal));
    EXPECT_TRUE(proxy.IsAxisUnbounded(Axis2D::Y, ViewSizeProposal::ideal));
}

TEST(ViewProxy, IsBounded) {
    TestBoundedView sut;
    
    auto proxy = sut.MakeViewProxy();
    EXPECT_FALSE(proxy.IsWidthUnbounded(ViewSizeProposal::ideal));
    EXPECT_FALSE(proxy.IsHeightUnbounded(ViewSizeProposal::ideal));
    EXPECT_FALSE(proxy.IsAxisUnbounded(Axis2D::X, ViewSizeProposal::ideal));
    EXPECT_FALSE(proxy.IsAxisUnbounded(Axis2D::Y, ViewSizeProposal::ideal));
}
