#include "gtest/gtest.h"
#include "TimeTrack.h"

using namespace PJ;
using namespace std;

namespace TimeTrackTests {
}

using namespace TimeTrackTests;

TEST(TimeTrack, AddReplace)
{
    TimeTrack<float, EaseKeyframe<float>> sut(10);
    auto i1 = sut.AddAt(1);
    EXPECT_EQ(i1, sut.Keyframes().begin());
    EXPECT_EQ(1, sut.Keyframes().size());

    auto i2 = sut.AddAt(1);
    EXPECT_EQ(i2, sut.Keyframes().begin());
    EXPECT_EQ(1, sut.Keyframes().size());
    EXPECT_EQ(i2->time, 1);
}

TEST(TimeTrack, AddBefore)
{
    TimeTrack<float, EaseKeyframe<float>> sut(10);
    auto i1 = sut.AddAt(1);
    EXPECT_EQ(i1, sut.Keyframes().begin());
    EXPECT_EQ(1, sut.Keyframes().size());

    auto i2 = sut.AddAt(0.1f);
    EXPECT_EQ(i2, sut.Keyframes().begin());
    EXPECT_EQ(2, sut.Keyframes().size());
    EXPECT_EQ(i2->time, 0.1f);
}

TEST(TimeTrack, AddAfter)
{
    TimeTrack<float, EaseKeyframe<float>> sut(10);
    auto i1 = sut.AddAt(0);
    EXPECT_EQ(i1, sut.Keyframes().begin());
    EXPECT_EQ(1, sut.Keyframes().size());

    auto i2 = sut.AddAt(0.3f);
    EXPECT_EQ(i2, sut.Keyframes().begin() + 1);
    EXPECT_EQ(2, sut.Keyframes().size());
    EXPECT_EQ(i2->time, 0.3f);
}

TEST(TimeTrack, AddBetween)
{
    TimeTrack<float, EaseKeyframe<float>> sut(10);
    auto i1 = sut.AddAt(0);
    EXPECT_EQ(i1, sut.Keyframes().begin());
    EXPECT_EQ(1, sut.Keyframes().size());

    auto i2 = sut.AddAt(1);
    EXPECT_EQ(2, sut.Keyframes().size());
    EXPECT_EQ(i2, sut.Keyframes().begin() + 1);

    auto i3 = sut.AddAt(0.5f);
    EXPECT_EQ(3, sut.Keyframes().size());
    EXPECT_EQ(i3, sut.Keyframes().begin() + 1);
}

TEST(TimeTrack, RemoveFirst)
{
    TimeTrack<float, EaseKeyframe<float>> sut(10);
    sut.AddAt(0);
    sut.AddAt(1);
    sut.AddAt(0.5f);
    EXPECT_EQ(3, sut.Keyframes().size());

    sut.RemoveAt(0);
    ASSERT_EQ(2, sut.Keyframes().size());
    EXPECT_EQ(0.5f, sut.Keyframes()[0].time);
    EXPECT_EQ(1, sut.Keyframes()[1].time);
}

TEST(TimeTrack, RemoveMiddle)
{
    TimeTrack<float, EaseKeyframe<float>> sut(10);
    sut.AddAt(0);
    sut.AddAt(1);
    sut.AddAt(0.5f);
    EXPECT_EQ(3, sut.Keyframes().size());

    sut.RemoveAt(0.5f);
    ASSERT_EQ(2, sut.Keyframes().size());
    EXPECT_EQ(0, sut.Keyframes()[0].time);
    EXPECT_EQ(1, sut.Keyframes()[1].time);
}

TEST(TimeTrack, RemoveFinal)
{
    TimeTrack<float, EaseKeyframe<float>> sut(10);
    sut.AddAt(0);
    sut.AddAt(1);
    sut.AddAt(0.5f);
    EXPECT_EQ(3, sut.Keyframes().size());

    sut.RemoveAt(1);
    ASSERT_EQ(2, sut.Keyframes().size());
    EXPECT_EQ(0, sut.Keyframes()[0].time);
    EXPECT_EQ(0.5f, sut.Keyframes()[1].time);
}

TEST(TimeTrack, KeyframeAt)
{
    TimeTrack<float, EaseKeyframe<float>> sut(10);
    sut.AddAt(0);
    sut.AddAt(1);
    sut.AddAt(0.5f);

    ASSERT_NE(nullptr, sut.KeyframeAt(1));
    EXPECT_EQ(nullptr, sut.KeyframeAt(0.3f));
    EXPECT_EQ(1, sut.KeyframeAt(1)->time);
}

TEST(TimeTrack, KeyframeAtOrAfter)
{
    TimeTrack<float, EaseKeyframe<float>> sut(10);
    sut.AddAt(0);
    sut.AddAt(1);
    sut.AddAt(0.5f);

    ASSERT_NE(nullptr, sut.KeyframeAtOrAfter(0.5f));
    EXPECT_EQ(nullptr, sut.KeyframeAtOrAfter(1.1f));
    EXPECT_EQ(1, sut.KeyframeAtOrAfter(0.6f)->time);
    EXPECT_EQ(1, sut.KeyframeAtOrAfter(1)->time);
}

TEST(TimeTrack, KeyframeBefore)
{
    TimeTrack<float, EaseKeyframe<float>> sut(10);
    sut.AddAt(0.2f);
    sut.AddAt(1);
    sut.AddAt(0.5f);

    ASSERT_NE(nullptr, sut.KeyframeBefore(2));
    EXPECT_EQ(nullptr, sut.KeyframeBefore(0.2f));
    EXPECT_EQ(1, sut.KeyframeBefore(1.5f)->time);
    EXPECT_EQ(0.2f, sut.KeyframeBefore(0.3f)->time);
}

TEST(TimeTrack, OnUpdateNoEase)
{
    TimeTrack<float, EaseKeyframe<float>> sut(10);
    sut.AddAt(0)->value = 10;
    sut.AddAt(10)->value = 20;

    float testValue{};
    sut.setBindingFunc = [&](float value) { testValue = value; };

    sut.OnUpdate({5});
    EXPECT_EQ(15, testValue);

    sut.OnUpdate({5});
    EXPECT_EQ(20, testValue);
}

TEST(TimeTrack, OnUpdateInEase)
{
    TimeTrack<float, EaseKeyframe<float>> sut(10);
    sut.AddAt(0)->value = 10;
    auto i2 = sut.AddAt(10);
    i2->value = 20;
    i2->core.inEaseFunc = [](auto value) { return value * value; };

    float testValue{};
    sut.setBindingFunc = [&](float value) { testValue = value; };

    sut.OnUpdate({5});
    EXPECT_EQ(0.5f*0.5f*10 + 10, testValue);

    sut.OnUpdate({5});
    EXPECT_EQ(20, testValue);
}

TEST(TimeTrack, OnUpdateOutEase)
{
    TimeTrack<float, EaseKeyframe<float>> sut(10);
    auto i1 = sut.AddAt(0);
    i1->value = 10;
    i1->core.outEaseFunc = [](auto value) { return value * value; };
    auto i2 = sut.AddAt(10);
    i2->value = 20;

    float testValue{};
    sut.setBindingFunc = [&](float value) { testValue = value; };

    sut.OnUpdate({5});
    EXPECT_EQ(0.5f*0.5f*10 + 10, testValue);

    sut.OnUpdate({5});
    EXPECT_EQ(20, testValue);
}

TEST(TimeTrack, OnUpdateDiscrete)
{
    TimeTrack<float, Keyframe<float>> sut(10, AnimationCycleType::Once, TimeTrackValueType::Discrete);
    sut.AddAt(2)->value = 10;
    sut.AddAt(10)->value = 20;

    float testValue{};
    sut.setBindingFunc = [&](float value) { testValue = value; };

    sut.OnUpdate({1});
    EXPECT_EQ(0, testValue);

    sut.OnUpdate({4});
    EXPECT_EQ(10, testValue);

    sut.OnUpdate({4});
    EXPECT_EQ(10, testValue);

    sut.OnUpdate({1});
    EXPECT_EQ(20, testValue);
}
