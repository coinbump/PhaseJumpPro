#include "gtest/gtest.h"
#include "Angle.h"
#include <memory>
#include "MathUtils.h"

using namespace PJ;
using namespace std;

namespace AngleTests {
}

using namespace AngleTests;

TEST(Angle, InitZero)
{
    auto sut = Angle::zero;
    EXPECT_EQ(0, sut.Radians());
    EXPECT_EQ(0, sut.Degrees());
}

TEST(Angle, InitDegrees)
{
    const float near = .001f;

    auto sut = Angle::DegreesAngle(360.0f);
    EXPECT_EQ(360.0f, sut.Degrees());
    EXPECT_NEAR(2.0f * M_PI, sut.Radians(), near);
}

TEST(Angle, InitRadians)
{
    auto sut = Angle::RadiansAngle(M_PI * 2.0f);
    EXPECT_EQ(360.0f, sut.Degrees());
}

TEST(Angle, Clipped)
{
    EXPECT_EQ(1, Angle::DegreesAngle(361).Clipped().Degrees());
    EXPECT_EQ(0, Angle::DegreesAngle(-360).Clipped().Degrees());
    EXPECT_EQ(359, Angle::DegreesAngle(-361).Clipped().Degrees());
    EXPECT_EQ(0, Angle::DegreesAngle(-720).Clipped().Degrees());
}

TEST(Angle, ToVector2)
{
    float sqrt2 = sqrt(2);

    const float near = .001f;

    auto adjOpp = Angle::DegreesAngle(45).ToVector2(sqrt2);
    EXPECT_NEAR(adjOpp.x, 1, .01);
    EXPECT_NEAR(adjOpp.y, 1 * vecUp, near);
    adjOpp = Angle::DegreesAngle(135).ToVector2(sqrt2);
    EXPECT_NEAR(adjOpp.x, 1, .01);
    EXPECT_NEAR(adjOpp.y, 1 * vecDown, near);
    adjOpp = Angle::DegreesAngle(225).ToVector2(sqrt2);
    EXPECT_NEAR(adjOpp.x, -1, .01);
    EXPECT_NEAR(adjOpp.y, 1 * vecDown, near);
    adjOpp = Angle::DegreesAngle(315).ToVector2(sqrt2);
    EXPECT_NEAR(adjOpp.x, -1, .01);
    EXPECT_NEAR(adjOpp.y, 1 * vecUp, near);

    // IMPORTANT: the angle specified is away from the 0 degree, *not* from the.x-coordinate line.
    // 60 degrees rotated away from origin 0 is a 30 degree right triangle.
    adjOpp = Angle::DegreesAngle(60).ToVector2(1);
    EXPECT_NEAR(adjOpp.x, .866, .01);
    EXPECT_NEAR(adjOpp.y, .5 * vecUp, .01);
}

TEST(Angle, InitVector2)
{
    const float near = .001f;

    Vector2 distance;
    Angle angle = Angle(distance);
    distance.x = 0;
    distance.y = 0;
    angle = Angle(distance);
    EXPECT_EQ(0, angle.Degrees());

    // TODO: fix this test (it's returning 360, which is equivalent to 0, but this isn't smart enough)
//    distance.x = 0;
//    distance.y = 10 * vecUp;
//    angle = Angle(distance);
//    EXPECT_NEAR(0, angle.Degrees(), near);
    distance.x = 10;
    distance.y = 0;
    angle = Angle(distance);
    EXPECT_NEAR(90, angle.Degrees(), near);
    distance.x = 0;
    distance.y = 10 * vecDown;
    angle = Angle(distance);
    EXPECT_NEAR(180, angle.Degrees(), near);
    distance.x = -10;
    distance.y = 0;
    angle = Angle(distance);
    EXPECT_NEAR(270, angle.Degrees(), near);

    distance.x = 10;
    distance.y = 10 * vecUp;
    angle = Angle(distance);
    EXPECT_NEAR(45, angle.Degrees(), near);
    distance.x = 10;
    distance.y = 10 * vecDown;
    angle = Angle(distance);
    EXPECT_NEAR(135, angle.Degrees(), near);
    distance.x = -10;
    distance.y = 10 * vecDown;
    angle = Angle(distance);
    EXPECT_NEAR(225, angle.Degrees(), near);
    distance.x = -10;
    distance.y = 10 * vecUp;
    angle = Angle(distance);
    EXPECT_NEAR(315, angle.Degrees(), near);
}

TEST(Angle, MinusOperator)
{
    auto sut = Angle::DegreesAngle(360);
    sut = -sut;
    EXPECT_EQ(-360.0f, sut.Degrees());
}

TEST(Angle, MinAngleTo)
{
    auto sut = Angle::DegreesAngle(360);
    auto sut2 = Angle::DegreesAngle(2);
    EXPECT_EQ(2.0f, sut.MinAngleTo(sut2).Degrees());
    EXPECT_EQ(-2.0f, sut2.MinAngleTo(sut).Degrees());
}
