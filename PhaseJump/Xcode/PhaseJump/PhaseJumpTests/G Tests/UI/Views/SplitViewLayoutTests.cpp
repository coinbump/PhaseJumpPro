#include "SplitViewLayout.h"
#include "View2D.h"
#include "World.h"
#include "WorldNode.h"
#include "gtest/gtest.h"

using namespace std;
using namespace PJ;

namespace {
    /// Builds a parent (100x100) with a split-view subject and `childCount` children, applies
    /// layout, and returns pointers to the subject + children for assertions.
    struct Harness {
        World world;
        SP<WorldNode> parentNode;
        SP<View2D> parentView;
        SP<WorldNode> subjectNode;
        SP<View2D> subjectView;
        VectorList<SP<View2D>> childViews;

        Harness(Vector2 size, SplitViewLayout* layoutOut, size_t childCount) {
            parentNode = MAKE<WorldNode>();
            parentView = MAKE<View2D>();
            world.Add(parentNode);
            parentNode->Add(parentView);

            subjectNode = MAKE<WorldNode>();
            subjectView = MAKE<View2D>();
            parentNode->Add(subjectNode);
            subjectNode->Add(subjectView);

            for (size_t i = 0; i < childCount; ++i) {
                auto node = MAKE<WorldNode>();
                auto view = MAKE<View2D>();
                subjectNode->Add(node);
                node->Add(view);
                childViews.push_back(view);
            }

            parentView->SetFrameSize(size);
        }
    };

    void InstallLayout(Harness& h, UP<SomeViewLayout>& layout) {
        h.subjectView->SetLayout(layout);
        h.subjectView->LayoutIfNeeded();
    }
} // namespace

TEST(SplitViewLayout, SingleChildFillsBounds) {
    Harness h({ 100, 100 }, nullptr, 1);
    UP<SomeViewLayout> layout = NEW<SplitViewLayout>(Axis2D::X);
    InstallLayout(h, layout);

    EXPECT_EQ(Vector2(0, 0), h.childViews[0]->Frame().origin);
    EXPECT_EQ(Vector2(100, 100), h.childViews[0]->Frame().size);
}

TEST(SplitViewLayout, TwoChildrenDefaultXAxis) {
    Harness h({ 100, 100 }, nullptr, 2);
    UP<SomeViewLayout> layout = NEW<SplitViewLayout>(Axis2D::X);
    InstallLayout(h, layout);

    EXPECT_EQ(Vector2(0, 0), h.childViews[0]->Frame().origin);
    EXPECT_EQ(Vector2(50, 100), h.childViews[0]->Frame().size);
    EXPECT_EQ(Vector2(50, 0), h.childViews[1]->Frame().origin);
    EXPECT_EQ(Vector2(50, 100), h.childViews[1]->Frame().size);
}

TEST(SplitViewLayout, TwoChildrenDefaultYAxis) {
    Harness h({ 100, 100 }, nullptr, 2);
    UP<SomeViewLayout> layout = NEW<SplitViewLayout>(Axis2D::Y);
    InstallLayout(h, layout);

    EXPECT_EQ(Vector2(0, 0), h.childViews[0]->Frame().origin);
    EXPECT_EQ(Vector2(100, 50), h.childViews[0]->Frame().size);
    EXPECT_EQ(Vector2(0, 50), h.childViews[1]->Frame().origin);
    EXPECT_EQ(Vector2(100, 50), h.childViews[1]->Frame().size);
}

TEST(SplitViewLayout, TwoChildrenCustomFirstViewSize) {
    Harness h({ 200, 100 }, nullptr, 2);
    UP<SomeViewLayout> layout = NEW<SplitViewLayout>(Axis2D::X, [](float length) {
        return length * 0.25f;
    });
    InstallLayout(h, layout);

    EXPECT_EQ(Vector2(50, 100), h.childViews[0]->Frame().size);
    EXPECT_EQ(Vector2(50, 0), h.childViews[1]->Frame().origin);
    EXPECT_EQ(Vector2(150, 100), h.childViews[1]->Frame().size);
}

TEST(SplitViewLayout, FirstViewSizeClampedToBounds) {
    Harness h({ 100, 100 }, nullptr, 2);
    UP<SomeViewLayout> layout = NEW<SplitViewLayout>(Axis2D::X, [](float) { return 500.0f; });
    InstallLayout(h, layout);

    // First gets clamped to the full axis length; second gets zero length.
    EXPECT_FLOAT_EQ(100, h.childViews[0]->Frame().size.x);
    EXPECT_FLOAT_EQ(0, h.childViews[1]->Frame().size.x);
}

TEST(SplitViewLayout, ThreeChildrenDefaultMiddleIs10) {
    Harness h({ 200, 100 }, nullptr, 3);
    UP<SomeViewLayout> layout = NEW<SplitViewLayout>(Axis2D::X);
    InstallLayout(h, layout);

    // First = 50% of 200 = 100. Middle = 10. Last = 90.
    EXPECT_EQ(Vector2(0, 0), h.childViews[0]->Frame().origin);
    EXPECT_EQ(Vector2(100, 100), h.childViews[0]->Frame().size);
    EXPECT_EQ(Vector2(100, 0), h.childViews[1]->Frame().origin);
    EXPECT_EQ(Vector2(10, 100), h.childViews[1]->Frame().size);
    EXPECT_EQ(Vector2(110, 0), h.childViews[2]->Frame().origin);
    EXPECT_EQ(Vector2(90, 100), h.childViews[2]->Frame().size);
}

TEST(SplitViewLayout, ThreeChildrenCustomFirstAndMiddle) {
    Harness h({ 200, 100 }, nullptr, 3);
    UP<SomeViewLayout> layout = NEW<SplitViewLayout>(
        Axis2D::X,
        [](float length) { return length * 0.3f; },
        [](float length) { return length * 0.1f; }
    );
    InstallLayout(h, layout);

    // First = 60, Middle = 20, Last = 120.
    EXPECT_FLOAT_EQ(60, h.childViews[0]->Frame().size.x);
    EXPECT_FLOAT_EQ(60, h.childViews[1]->Frame().origin.x);
    EXPECT_FLOAT_EQ(20, h.childViews[1]->Frame().size.x);
    EXPECT_FLOAT_EQ(80, h.childViews[2]->Frame().origin.x);
    EXPECT_FLOAT_EQ(120, h.childViews[2]->Frame().size.x);
}

TEST(SplitViewLayout, ThreeChildrenYAxis) {
    Harness h({ 100, 200 }, nullptr, 3);
    UP<SomeViewLayout> layout = NEW<SplitViewLayout>(Axis2D::Y);
    InstallLayout(h, layout);

    // First = 100, Middle = 10, Last = 90.
    EXPECT_EQ(Vector2(100, 100), h.childViews[0]->Frame().size);
    EXPECT_FLOAT_EQ(100, h.childViews[1]->Frame().origin.y);
    EXPECT_FLOAT_EQ(10, h.childViews[1]->Frame().size.y);
    EXPECT_FLOAT_EQ(110, h.childViews[2]->Frame().origin.y);
    EXPECT_FLOAT_EQ(90, h.childViews[2]->Frame().size.y);
}

TEST(SplitViewLayout, MiddleClampedWhenFirstTakesAllSpace) {
    Harness h({ 100, 100 }, nullptr, 3);
    UP<SomeViewLayout> layout =
        NEW<SplitViewLayout>(Axis2D::X, [](float length) { return length; });
    InstallLayout(h, layout);

    // First takes everything, middle/last clamp to 0.
    EXPECT_FLOAT_EQ(100, h.childViews[0]->Frame().size.x);
    EXPECT_FLOAT_EQ(0, h.childViews[1]->Frame().size.x);
    EXPECT_FLOAT_EQ(0, h.childViews[2]->Frame().size.x);
}
