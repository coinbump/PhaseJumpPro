#include "gtest/gtest.h"
#include "Animator.h"

using namespace PJ;
using namespace std;

namespace AnimatorTests {
}

using namespace AnimatorTests;

TEST(Animator, Animate)
{
    auto testValue = -1.0f;

    auto interpolator = Interpolator::MakeEaseFunc(Interpolator::MakeFunc<float>(0.0f, 360.0f));
    SetBindingFunc<float> binding = [&testValue](float value) { testValue = value; };
    Animator<float> sut(interpolator,
                        1.0f,
                        binding);

    sut.OnUpdate(TimeSlice(0.5f));
    EXPECT_EQ(0.5f, sut.Progress());
    EXPECT_EQ(180.0f, testValue);
    EXPECT_FALSE(sut.IsFinished());

    sut.OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(1.0f, sut.Progress());
    EXPECT_EQ(360.0f, testValue);
    EXPECT_TRUE(sut.IsFinished());
}
