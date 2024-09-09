#include "gtest/gtest.h"
#include "Bounds.h"

using namespace PJ;
using namespace std;

namespace BoundsTests {
}

using namespace BoundsTests;

TEST(Bounds, TestHit)
{
    Bounds sut(Vector3(0, 1, -1), Vector3(1, 2, 1));
    EXPECT_TRUE(sut.TestHit(Vector3(0, 0, 0)));
    EXPECT_TRUE(sut.TestHit(Vector3(1, 3, 0)));
    EXPECT_TRUE(sut.TestHit(Vector3(-1, -1, 0)));
    EXPECT_FALSE(sut.TestHit(Vector3(-1, -1, 1)));
    EXPECT_TRUE(sut.TestHit(Vector3(-1, -1, -2)));
    EXPECT_FALSE(sut.TestHit(Vector3(-1, -1, -3)));
    EXPECT_FALSE(sut.TestHit(Vector3(-2, -1, 0)));
    EXPECT_FALSE(sut.TestHit(Vector3(-1, -2, 0)));
    EXPECT_FALSE(sut.TestHit(Vector3(2, 3, 0)));
}

TEST(Bounds, TestHitCollide)
{
    Bounds sut(Vector3(0, 0, 0), Vector3(2, 2, 2));
    
    EXPECT_TRUE(sut.TestCollide(Bounds(Vector3(0, 0, 0), Vector3(1, 1, 1))));
    EXPECT_TRUE(sut.TestCollide(Bounds(Vector3(1, 1, 1), Vector3(1, 1, 1))));
    EXPECT_TRUE(sut.TestCollide(Bounds(Vector3(-1, 1, -1), Vector3(1, 1, 1))));
    EXPECT_TRUE(sut.TestCollide(Bounds(Vector3(2, 2, 2), Vector3(1, 1, 1))));
    EXPECT_FALSE(sut.TestCollide(Bounds(Vector3(4, 3, 3), Vector3(1, 1, 1))));
}
