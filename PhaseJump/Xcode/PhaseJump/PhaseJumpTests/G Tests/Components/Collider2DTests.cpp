#include "gtest/gtest.h"

#include "Colliders2D.h"

using namespace std;
using namespace PJ;

namespace Collider2DTests {
};

TEST(CircleCollider2D, Test) {
    CircleCollider2D sut(10);

    EXPECT_TRUE(sut.TestHit(Vector2(0, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(10, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(7.071, 7.071)));
    EXPECT_TRUE(sut.TestHit(Vector2(0, 10)));
    EXPECT_FALSE(sut.TestHit(Vector2(10, 10)));
}
