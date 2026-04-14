#include "gtest/gtest.h"
#include "BezierPathLayout.h"
#include "WorldNode.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace BezierPathLayoutTests {
}

using namespace BezierPathLayoutTests;

TEST(BezierPathLayout, TypeName)
{
    auto sut = MAKE<BezierPathLayout>();
    EXPECT_EQ("BezierPathLayout", sut->TypeName());
}

TEST(BezierPathLayout, ConstructWithControlPoints)
{
    VectorList<Vector3> points = {
        Vector3(0, 0, 0),
        Vector3(5, 10, 0),
        Vector3(10, 0, 0)
    };
    auto sut = MAKE<BezierPathLayout>(points);

    auto path = sut->BuildPath();
    ASSERT_NE(nullptr, path);

    // Progress 0 -> first control point
    auto pos = path->PositionAt(0);
    EXPECT_NEAR(0, pos.x, 0.001f);
    EXPECT_NEAR(0, pos.y, 0.001f);

    // Progress 1 -> last control point
    pos = path->PositionAt(1.0f);
    EXPECT_NEAR(10.0f, pos.x, 0.001f);
    EXPECT_NEAR(0, pos.y, 0.001f);
}

TEST(BezierPathLayout, SetControlPoints)
{
    auto sut = MAKE<BezierPathLayout>();

    VectorList<Vector3> points = {
        Vector3(0, 0, 0),
        Vector3(5, 10, 0),
        Vector3(10, 0, 0)
    };
    sut->SetControlPoints(points);

    auto path = sut->BuildPath();
    ASSERT_NE(nullptr, path);

    auto pos = path->PositionAt(0);
    EXPECT_NEAR(0, pos.x, 0.001f);
    EXPECT_NEAR(0, pos.y, 0.001f);
}

TEST(BezierPathLayout, SetControlPointsSetsNeedsLayout)
{
    auto worldNode = MAKE<WorldNode>();
    auto sut = MAKE<BezierPathLayout>();
    worldNode->Add(sut);

    sut->LayoutIfNeeded();
    EXPECT_FALSE(sut->NeedsLayout());

    VectorList<Vector3> points = {
        Vector3(0, 0, 0),
        Vector3(5, 10, 0),
        Vector3(10, 0, 0)
    };
    sut->SetControlPoints(points);
    EXPECT_TRUE(sut->NeedsLayout());
}

TEST(BezierPathLayout, SetControlPointSetsNeedsLayout)
{
    VectorList<Vector3> points = {
        Vector3(0, 0, 0),
        Vector3(5, 10, 0),
        Vector3(10, 0, 0)
    };

    auto worldNode = MAKE<WorldNode>();
    auto sut = MAKE<BezierPathLayout>(points);
    worldNode->Add(sut);

    sut->LayoutIfNeeded();
    EXPECT_FALSE(sut->NeedsLayout());

    sut->SetControlPoint(1, Vector3(5, 20, 0));
    EXPECT_TRUE(sut->NeedsLayout());
}

TEST(BezierPathLayout, SetControlPointInvalidIndex)
{
    VectorList<Vector3> points = {
        Vector3(0, 0, 0),
        Vector3(5, 10, 0),
        Vector3(10, 0, 0)
    };

    auto worldNode = MAKE<WorldNode>();
    auto sut = MAKE<BezierPathLayout>(points);
    worldNode->Add(sut);

    sut->LayoutIfNeeded();
    EXPECT_FALSE(sut->NeedsLayout());

    // Out of bounds index should not modify state
    sut->SetControlPoint(5, Vector3(99, 99, 0));
    EXPECT_FALSE(sut->NeedsLayout());
}

TEST(BezierPathLayout, QuadraticPathMidpoint)
{
    VectorList<Vector3> points = {
        Vector3(0, 0, 0),
        Vector3(5, 10, 0),
        Vector3(10, 0, 0)
    };
    auto sut = MAKE<BezierPathLayout>(points);

    auto path = sut->BuildPath();
    auto pos = path->PositionAt(0.5f);
    EXPECT_NEAR(5.0f, pos.x, 0.001f);
    EXPECT_NEAR(5.0f, pos.y, 0.001f);
}

TEST(BezierPathLayout, CubicPathEndpoints)
{
    VectorList<Vector3> points = {
        Vector3(0, 0, 0),
        Vector3(3, 10, 0),
        Vector3(7, 10, 0),
        Vector3(10, 0, 0)
    };
    auto sut = MAKE<BezierPathLayout>(points);

    auto path = sut->BuildPath();
    ASSERT_NE(nullptr, path);

    auto pos = path->PositionAt(0);
    EXPECT_NEAR(0, pos.x, 0.001f);
    EXPECT_NEAR(0, pos.y, 0.001f);

    pos = path->PositionAt(1.0f);
    EXPECT_NEAR(10.0f, pos.x, 0.001f);
    EXPECT_NEAR(0, pos.y, 0.001f);
}

TEST(BezierPathLayout, CubicPathMidpoint)
{
    VectorList<Vector3> points = {
        Vector3(0, 0, 0),
        Vector3(0, 10, 0),
        Vector3(10, 10, 0),
        Vector3(10, 0, 0)
    };
    auto sut = MAKE<BezierPathLayout>(points);

    auto path = sut->BuildPath();
    auto pos = path->PositionAt(0.5f);
    EXPECT_NEAR(5.0f, pos.x, 0.001f);
    EXPECT_NEAR(7.5f, pos.y, 0.001f);
}

TEST(BezierPathLayout, EmptyControlPointsReturnsZero)
{
    auto sut = MAKE<BezierPathLayout>();

    auto path = sut->BuildPath();
    ASSERT_NE(nullptr, path);

    // No control points -> returns zero vector
    auto pos = path->PositionAt(0.5f);
    EXPECT_EQ(0, pos.x);
    EXPECT_EQ(0, pos.y);
    EXPECT_EQ(0, pos.z);
}

TEST(BezierPathLayout, LayoutChildrenQuadratic)
{
    VectorList<Vector3> points = {
        Vector3(0, 0, 0),
        Vector3(5, 10, 0),
        Vector3(10, 0, 0)
    };

    auto worldNode = MAKE<WorldNode>();
    auto sut = MAKE<BezierPathLayout>(points);

    auto child1 = MAKE<WorldNode>();
    auto child2 = MAKE<WorldNode>();
    auto child3 = MAKE<WorldNode>();

    worldNode->Add(child1);
    worldNode->Add(child2);
    worldNode->Add(child3);
    worldNode->Add(sut);

    sut->ApplyLayout();

    // 3 children: progress 0, 0.5, 1
    // Progress 0 -> p0 = (0, 0)
    auto pos1 = (Vector2)child1->transform.LocalPosition();
    EXPECT_NEAR(0, pos1.x, 0.01f);
    EXPECT_NEAR(0, pos1.y, 0.01f);

    // Progress 0.5 -> (5, 5) (see QuadraticPathMidpoint test)
    auto pos2 = (Vector2)child2->transform.LocalPosition();
    EXPECT_NEAR(5.0f, pos2.x, 0.01f);
    EXPECT_NEAR(5.0f, pos2.y, 0.01f);

    // Progress 1 -> p2 = (10, 0)
    auto pos3 = (Vector2)child3->transform.LocalPosition();
    EXPECT_NEAR(10.0f, pos3.x, 0.01f);
    EXPECT_NEAR(0, pos3.y, 0.01f);
}

TEST(BezierPathLayout, LayoutSingleChild)
{
    VectorList<Vector3> points = {
        Vector3(0, 0, 0),
        Vector3(5, 10, 0),
        Vector3(10, 0, 0)
    };

    auto worldNode = MAKE<WorldNode>();
    auto sut = MAKE<BezierPathLayout>(points);

    auto child = MAKE<WorldNode>();
    worldNode->Add(child);
    worldNode->Add(sut);

    sut->ApplyLayout();

    // Single child: normalOffset is 0, so progress = 0 -> p0
    auto pos = (Vector2)child->transform.LocalPosition();
    EXPECT_NEAR(0, pos.x, 0.01f);
    EXPECT_NEAR(0, pos.y, 0.01f);
}
