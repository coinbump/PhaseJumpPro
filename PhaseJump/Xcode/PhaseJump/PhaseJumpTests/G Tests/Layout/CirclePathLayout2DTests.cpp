#include "gtest/gtest.h"
#include "CirclePathLayout2D.h"
#include "WorldNode.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace CirclePathLayout2DTests {
}

using namespace CirclePathLayout2DTests;

TEST(CirclePathLayout2D, DefaultRadius)
{
    auto sut = MAKE<CirclePathLayout2D>();
    EXPECT_EQ(1.0f, sut->Radius());
}

TEST(CirclePathLayout2D, CustomRadius)
{
    auto sut = MAKE<CirclePathLayout2D>(5.0f);
    EXPECT_EQ(5.0f, sut->Radius());
}

TEST(CirclePathLayout2D, SetRadius)
{
    auto sut = MAKE<CirclePathLayout2D>();
    sut->SetRadius(3.0f);
    EXPECT_EQ(3.0f, sut->Radius());
}

TEST(CirclePathLayout2D, SetRadiusSameValue)
{
    auto worldNode = MAKE<WorldNode>();
    auto sut = MAKE<CirclePathLayout2D>(2.0f);
    worldNode->Add(sut);

    sut->LayoutIfNeeded();

    sut->SetRadius(2.0f);
    EXPECT_FALSE(sut->NeedsLayout());
}

TEST(CirclePathLayout2D, WorldSize)
{
    auto sut = MAKE<CirclePathLayout2D>(5.0f);
    auto worldSize = sut->WorldSize();
    EXPECT_EQ(10.0f, worldSize.x);
    EXPECT_EQ(10.0f, worldSize.y);
    EXPECT_EQ(0, worldSize.z);
}

TEST(CirclePathLayout2D, SetWorldSize)
{
    auto sut = MAKE<CirclePathLayout2D>();
    sut->SetWorldSize(Vector3(10.0f, 20.0f, 0));

    EXPECT_EQ(5.0f, sut->Radius());
}

TEST(CirclePathLayout2D, SetWorldSizeUsesMinDimension)
{
    auto sut = MAKE<CirclePathLayout2D>();
    sut->SetWorldSize(Vector3(20.0f, 6.0f, 0));

    EXPECT_EQ(3.0f, sut->Radius());
}

TEST(CirclePathLayout2D, BuildPath)
{
    auto sut = MAKE<CirclePathLayout2D>(4.0f);
    auto path = sut->BuildPath();

    ASSERT_NE(nullptr, path);

    // Progress 0: angle 0 -> top of circle (0, radius)
    auto pos = path->PositionAt(0);
    EXPECT_NEAR(0, pos.x, 0.001f);
    EXPECT_NEAR(4.0f, pos.y, 0.001f);

    // Progress 0.25: angle 90 -> right of circle (radius, 0)
    pos = path->PositionAt(0.25f);
    EXPECT_NEAR(4.0f, pos.x, 0.001f);
    EXPECT_NEAR(0, pos.y, 0.001f);

    // Progress 0.5: angle 180 -> bottom of circle (0, -radius)
    pos = path->PositionAt(0.5f);
    EXPECT_NEAR(0, pos.x, 0.001f);
    EXPECT_NEAR(-4.0f, pos.y, 0.001f);
}

TEST(CirclePathLayout2D, TypeName)
{
    auto sut = MAKE<CirclePathLayout2D>();
    EXPECT_EQ("CirclePathLayout2D", sut->TypeName());
}

TEST(CirclePathLayout2D, LayoutWithChildren)
{
    auto worldNode = MAKE<WorldNode>();
    auto sut = MAKE<CirclePathLayout2D>(10.0f);

    auto child1 = MAKE<WorldNode>();
    auto child2 = MAKE<WorldNode>();

    worldNode->Add(child1);
    worldNode->Add(child2);
    worldNode->Add(sut);

    sut->ApplyLayout();

    // 2 children: positioned at progress 0 and 1 (same point on a circle)
    // Progress 0: angle 0 -> top of circle (0, radius)
    // Progress 1: angle 360 -> top of circle (0, radius)
    auto pos1 = (Vector2)child1->transform.LocalPosition();
    auto pos2 = (Vector2)child2->transform.LocalPosition();

    EXPECT_NEAR(0, pos1.x, 0.01f);
    EXPECT_NEAR(10.0f, pos1.y, 0.01f);

    EXPECT_NEAR(0, pos2.x, 0.01f);
    EXPECT_NEAR(10.0f, pos2.y, 0.01f);
}

TEST(CirclePathLayout2D, LayoutFourChildren)
{
    auto worldNode = MAKE<WorldNode>();
    auto sut = MAKE<CirclePathLayout2D>(10.0f);

    auto child1 = MAKE<WorldNode>();
    auto child2 = MAKE<WorldNode>();
    auto child3 = MAKE<WorldNode>();
    auto child4 = MAKE<WorldNode>();

    worldNode->Add(child1);
    worldNode->Add(child2);
    worldNode->Add(child3);
    worldNode->Add(child4);
    worldNode->Add(sut);

    sut->ApplyLayout();

    // 4 children: progress 0, 1/3, 2/3, 1
    // x = sin(angle) * radius, y = cos(angle) * radius
    // Progress 0: angle 0 -> (0, 10)
    auto pos1 = (Vector2)child1->transform.LocalPosition();
    EXPECT_NEAR(0, pos1.x, 0.01f);
    EXPECT_NEAR(10.0f, pos1.y, 0.01f);

    // Progress 1/3: angle 120 -> (sin(120)*10, cos(120)*10) = (8.66, -5)
    auto pos2 = (Vector2)child2->transform.LocalPosition();
    EXPECT_NEAR(8.66f, pos2.x, 0.1f);
    EXPECT_NEAR(-5.0f, pos2.y, 0.1f);

    // Progress 2/3: angle 240 -> (sin(240)*10, cos(240)*10) = (-8.66, -5)
    auto pos3 = (Vector2)child3->transform.LocalPosition();
    EXPECT_NEAR(-8.66f, pos3.x, 0.1f);
    EXPECT_NEAR(-5.0f, pos3.y, 0.1f);

    // Progress 1: angle 360 -> (0, 10)
    auto pos4 = (Vector2)child4->transform.LocalPosition();
    EXPECT_NEAR(0, pos4.x, 0.01f);
    EXPECT_NEAR(10.0f, pos4.y, 0.01f);
}

TEST(CirclePathLayout2D, LayoutSingleChild)
{
    auto worldNode = MAKE<WorldNode>();
    auto sut = MAKE<CirclePathLayout2D>(5.0f);

    auto child = MAKE<WorldNode>();
    worldNode->Add(child);
    worldNode->Add(sut);

    sut->ApplyLayout();

    // Single child: normalOffset is 0, so progress = 0
    // Angle 0 -> top of circle (0, 5)
    auto pos = (Vector2)child->transform.LocalPosition();
    EXPECT_NEAR(0, pos.x, 0.01f);
    EXPECT_NEAR(5.0f, pos.y, 0.01f);
}
