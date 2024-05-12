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
    EXPECT_EQ(Vector3(0, 0, 0), sut.Position());
    sut.SetPosition(Vector3(1, 1, 1));
    EXPECT_EQ(Vector3(1, 1, 1), sut.Position());
}

TEST(SomePosition, LocalPosition)
{
    LocalPosition sut;
    EXPECT_EQ(Vector3(0, 0, 0), sut.Position());
    sut.SetPosition(Vector3(1, 1, 1));
    EXPECT_EQ(Vector3(1, 1, 1), sut.Position());
}

TEST(SomePosition, ScreenPosition)
{
    ScreenPosition sut;
    EXPECT_EQ(Vector3(0, 0, 0), sut.Position());
    sut.SetPosition(Vector3(1, 1, 1));
    EXPECT_EQ(Vector3(1, 1, 0), sut.Position());
}
