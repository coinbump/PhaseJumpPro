#include "gtest/gtest.h"
#include "Vector2.h"
#include <memory>

using namespace std;
using namespace PJ;

namespace Vector2Tests {
}

using namespace Vector2Tests;

TEST(Vector2, Init) {
    Vector2 sut;
    EXPECT_EQ(0, sut[0]);
    EXPECT_EQ(0, sut[1]);

    sut = Vector2(3, 5);
    EXPECT_EQ(3, sut[0]);
    EXPECT_EQ(5, sut[1]);
}

TEST(Vector2, Operators) {
    Vector2 sut(2, 2);
    EXPECT_EQ(Vector2(10, 10), sut * 5.0f);
    EXPECT_EQ(Vector2(1, 1), sut / 2.0f);

    EXPECT_EQ(Vector2(3, 3), sut + Vector2(1, 1));
    EXPECT_EQ(Vector2(1, 1), sut - Vector2(1, 1));
    EXPECT_EQ(Vector2(4, 4), sut * Vector2(2, 2));
    EXPECT_EQ(Vector2(1, 1), sut / Vector2(2, 2));

    sut *= 2;
    EXPECT_EQ(Vector2(4, 4), sut);

    sut /= 2;
    EXPECT_EQ(Vector2(2, 2), sut);

    sut *= Vector2(2, 2);
    EXPECT_EQ(Vector2(4, 4), sut);

    sut /= Vector2(2, 2);
    EXPECT_EQ(Vector2(2, 2), sut);

    sut += Vector2(2, 2);
    EXPECT_EQ(Vector2(4, 4), sut);

    sut -= Vector2(2, 2);
    EXPECT_EQ(Vector2(2, 2), sut);
}

TEST(Vector2, Magnitude) {
    Vector2 sut(0, 100);
    EXPECT_EQ(100, sut.Magnitude());

    sut = Vector2(1, 1);
    EXPECT_NEAR(1.414f, sut.Magnitude(), .001f);

    sut = Vector2(2, 2);
    EXPECT_NEAR(8.0f, sut.Magnitude(false), .001f);
}

TEST(Vector2, Normalize) {
    Vector2 sut(0, 100);
    sut.Normalize();
    EXPECT_EQ(Vector2(0, 1), sut);
}
