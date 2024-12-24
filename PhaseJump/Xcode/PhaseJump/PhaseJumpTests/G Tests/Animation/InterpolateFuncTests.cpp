#include "gtest/gtest.h"
#include "InterpolateFunc.h"

using namespace PJ;
using namespace std;

namespace InterpolateFuncTests {
}

using namespace InterpolateFuncTests;

TEST(Interpolator, LinearCurve)
{
    auto sut = InterpolateFuncs::Ease(InterpolateFuncs::Make<float>(0.0f, 100.0f));
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
    auto sut = InterpolateFuncs::Ease(vf);

    EXPECT_EQ(0, sut(0));
    EXPECT_EQ(100.0f, sut(0.5));
    EXPECT_EQ(200.0f, sut(1.0));
}

TEST(Interpolator, Reverse)
{
    float start = 0.0f;
    float end = 100.0f;
    auto sut = InterpolateFuncs::Reverse(InterpolateFuncs::Make(start, end));

    EXPECT_EQ(100.0f, sut(0));
    EXPECT_EQ(50.0f, sut(0.5f));
    EXPECT_EQ(0, sut(1.0f));
}

TEST(Interpolator, MakeBinding)
{
    float testValue = -1;
    float start = 0.0f;
    float end = 100.0f;
    InterpolateFunc<float> easeFunc = [=](float value) {
        return value * 2.0f;
    };
    auto interpolateFunc =
    InterpolateFuncs::Ease(InterpolateFuncs::Make(start, end), easeFunc);
    auto sut = InterpolateFuncs::Binding<float>(interpolateFunc, [&](auto& value) {
        testValue = value;
    });

    sut(0);
    EXPECT_EQ(0, testValue);
    sut(0.5f);
    EXPECT_EQ(100.0f, testValue);
    sut(1);
    EXPECT_EQ(200.0f, testValue);
}
