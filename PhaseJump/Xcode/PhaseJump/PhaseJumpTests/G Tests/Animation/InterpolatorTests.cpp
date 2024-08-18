#include "gtest/gtest.h"
#include "Interpolator.h"

using namespace PJ;
using namespace std;

namespace InterpolatorTests {
}

using namespace InterpolatorTests;

TEST(Interpolator, LinearCurve)
{
    auto sut = Interpolator::MakeEaseFunc(Interpolator::MakeFunc<float>(0.0f, 100.0f));
    EXPECT_EQ(0, sut(0));
    EXPECT_EQ(50.0f, sut(0.5));
    EXPECT_EQ(100.0f, sut(1.0));
}

TEST(Interpolator, DoubleCurve)
{
    float start = 0.0f;
    float end = 100.0f;
    InterpolateFunc<float> vf = [=](float progress) {
        return (start + (end - start) * progress) * 2.0f;
    };
    auto sut = Interpolator::MakeEaseFunc(vf);

    EXPECT_EQ(0, sut(0));
    EXPECT_EQ(100.0f, sut(0.5));
    EXPECT_EQ(200.0f, sut(1.0));
}
