#include "gtest/gtest.h"
#include "SomePosition.h"

using namespace PJ;
using namespace std;

namespace SomePositionTests {
}

using namespace SomePositionTests;

TEST(SomePosition, WorldPosition)
{
    WorldPosition sut;
    EXPECT_EQ(Vector3(0, 0, 0), sut);
    sut = Vector3(1, 1, 1);
    EXPECT_EQ(Vector3(1, 1, 1), sut);
}

TEST(SomePosition, LocalPosition)
{
    LocalPosition sut;
    EXPECT_EQ(Vector3(0, 0, 0), sut);
    sut = Vector3(1, 1, 1);
    EXPECT_EQ(Vector3(1, 1, 1), sut);
}

TEST(SomePosition, ScreenPosition)
{
    ScreenPosition sut;
    EXPECT_EQ(Vector2(0, 0), sut);
    sut.SetPosition(Vector3(1, 1, 1));
    EXPECT_EQ(Vector2(1, 1), sut);
}
