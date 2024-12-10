#include "gtest/gtest.h"
#include "World.h"
#include "WorldNode.h"
#include "WorldNodeTransform.h"

using namespace PJ;
using namespace std;

namespace WorldNodeTransformTests {
}

using namespace WorldNodeTransformTests;

TEST(WorldNodeTransform, WorldPosition)
{
    World world;
    auto n1 = MAKE<WorldNode>();
    auto n2 = MAKE<WorldNode>();
    auto n3 = MAKE<WorldNode>();

    n1->transform.SetLocalPosition(Vector3(-10, 10, 0));
    n2->transform.SetLocalPosition(Vector3(5, -5, 0));
    n3->transform.SetLocalPosition(Vector3(4, 4, 0));

    world.Add(n1);
    n1->Add(n2);
    n2->Add(n3);

    EXPECT_EQ(Vector3(-10, 10, 0), n1->transform.LocalPosition());
    EXPECT_EQ(Vector3(-10, 10, 0), n1->transform.WorldPosition());

    EXPECT_EQ(Vector3(5, -5, 0), n2->transform.LocalPosition());
    EXPECT_EQ(Vector3(-5, 5, 0), n2->transform.WorldPosition());

    EXPECT_EQ(Vector3(4, 4, 0), n3->transform.LocalPosition());
    EXPECT_EQ(Vector3(-1, 9, 0), n3->transform.WorldPosition());
}

TEST(WorldNodeTransform, SetWorldPosition)
{
    World world;
    auto n1 = MAKE<WorldNode>();
    auto n2 = MAKE<WorldNode>();
    auto n3 = MAKE<WorldNode>();

    n1->transform.SetLocalPosition({});
    n2->transform.SetLocalPosition({});
    n3->transform.SetLocalPosition({});

    world.Add(n1);
    n1->Add(n2);
    n2->Add(n3);

    n1->transform.SetWorldPosition(Vector3(0, -10, 10));
    EXPECT_EQ(Vector3(0, -10, 10), n1->transform.LocalPosition());
    EXPECT_EQ(Vector3(0, -10, 10), n1->transform.WorldPosition());

    n2->transform.SetWorldPosition(Vector3(0, -5, 5));
    EXPECT_EQ(Vector3(0, 5, -5), n2->transform.LocalPosition());
    EXPECT_EQ(Vector3(0, -5, 5), n2->transform.WorldPosition());

    n3->transform.SetWorldPosition(Vector3(0, -1, 9));
    EXPECT_EQ(Vector3(0, 4, 4), n3->transform.LocalPosition());
    EXPECT_EQ(Vector3(0, -1, 9), n3->transform.WorldPosition());
}

TEST(WorldNodeTransform, SetWorldPositionXY)
{
    World world;
    auto n1 = MAKE<WorldNode>();
    auto n2 = MAKE<WorldNode>();
    auto n3 = MAKE<WorldNode>();

    n1->transform.SetLocalPosition({});
    n2->transform.SetLocalPosition({});
    n3->transform.SetLocalPosition({});

    world.Add(n1);
    n1->Add(n2);
    n2->Add(n3);

    n1->transform.SetWorldPositionXY(Vector3(0, -10, 10));
    EXPECT_EQ(Vector3(0, -10, 0), n1->transform.WorldPosition());
}
