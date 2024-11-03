#include "gtest/gtest.h"
#include "Timeline.h"
#include "TimeTrack.h"

using namespace PJ;
using namespace std;

namespace TimelineTests {
}

using namespace TimelineTests;

TEST(Timeline, OnPlayTimeChangeWhenTimelineDrives)
{
    using TimeTrack = PJ::TimeTrack<float>;

    Timeline::Config config{
        .duration = 10,
        .cycleType = AnimationCycleType::Once
    };
    Timeline sut(config);

    EXPECT_EQ(10, sut.Duration());
    TimeTrack::Config trackConfig{.duration = 10};
    UP<SomeTimeTrack> track1 = NEW<TimeTrack>(trackConfig);
    UP<SomeTimeTrack> track2 = NEW<TimeTrack>(trackConfig);

    TimeTrack* track1Ptr = static_cast<TimeTrack*>(track1.get());
    TimeTrack* track2Ptr = static_cast<TimeTrack*>(track2.get());

    sut.Add(track1);
    sut.Add(track2);

    ASSERT_EQ(2, sut.Tracks().size());

    track1Ptr->AddAt(0)->value = 10;
    track1Ptr->AddAt(10)->value = 20;
    track2Ptr->AddAt(0)->value = 100;
    track2Ptr->AddAt(10)->value = 200;

    float value1{};
    float value2{};
    track1Ptr->setBindingFunc = [&](auto& value) { value1 = value; };
    track2Ptr->setBindingFunc = [&](auto& value) { value2 = value; };

    sut.OnUpdate({5});
    EXPECT_EQ(value1, 15);
    EXPECT_EQ(value2, 150);
}

TEST(Timeline, PingPong)
{
    Timeline::Config config{
        .duration = 3,
        .cycleType = AnimationCycleType::PingPong
    };
    Timeline sut(config);

    sut.AddKeyframe<float>("test", 0, {.keyframeConfig = {.value = 2}});
    sut.AddKeyframe<float>("test", 1, {.keyframeConfig = {.value = 3}});
    sut.AddKeyframe<float>("test", 2, {.keyframeConfig = {.value = 2}});
    sut.AddKeyframe<float>("test", 3, {.keyframeConfig = {.value = 1}});

    float testValue{};
    auto& track = *(static_cast<TimeTrack<float>*>(sut.Tracks()[0].get()));
    track.setBindingFunc = [&](float value) { testValue = value; };

    sut.OnUpdate({1});
    EXPECT_EQ(3, testValue);

    sut.OnUpdate({1});
    EXPECT_EQ(2, testValue);

    sut.OnUpdate({1});
    EXPECT_EQ(1, testValue);

    // Reverse happens here

    sut.OnUpdate({0.5f});
    EXPECT_EQ(1.5f, testValue);

    sut.OnUpdate({0.5f});
    EXPECT_EQ(2, testValue);

    sut.OnUpdate({0.5f});
    EXPECT_EQ(2.5f, testValue);

    sut.OnUpdate({0.5f});
    EXPECT_EQ(3.0f, testValue);

    sut.OnUpdate({0.5f});
    EXPECT_EQ(2.5f, testValue);

    sut.OnUpdate({0.5f});
    EXPECT_EQ(2, testValue);

    // Reverse happens here
}

TEST(Timeline, AddKeyframe)
{
    Timeline::Config config{
        .duration = 10,
        .cycleType = AnimationCycleType::Once
    };
    Timeline sut(config);
    sut.AddKeyframe<float>("value", 0, 3);
    sut.AddKeyframe<float>("value", 1, 4);

    auto _track = sut.FindTrack("value");
    ASSERT_NE(nullptr, _track);

    auto track = dynamic_cast<TimeTrack<float>*>(_track);
    ASSERT_NE(nullptr, _track);

    EXPECT_EQ(3, track->ValueAt(0));
    EXPECT_EQ(4, track->ValueAt(1));
}

TEST(Timeline, SetTrackBindingWithTimeTypeDiscrete)
{
    Timeline::Config config{
        .duration = 10,
        .cycleType = AnimationCycleType::Once,
    };
    Timeline sut(config);

    TimelineKeyframeConfig<float> kfConfig{
        .keyframeConfig = {.value = 0},
        .timeType = KeyedTimeType::Discrete
    };
    sut.AddKeyframe<float>("value", 0, kfConfig);
    sut.AddKeyframe<float>("value", 10, 10);

    float value = -1;
    sut.SetTrackBinding<float>("value", [&](auto _value) {
        value = _value;
    });

    EXPECT_EQ(0, value);

    auto track = sut.FindTrack("value");
    ASSERT_NE(nullptr, track);

    track->SetPlayTime(5);
    EXPECT_EQ(0, value);

    track->SetPlayTime(10);
    EXPECT_EQ(10, value);
}

TEST(Timeline, SetTrackBindingWithTimeTypeInterpolate)
{
    Timeline::Config config{
        .duration = 10,
        .cycleType = AnimationCycleType::Once
    };
    Timeline sut(config);

    TimelineKeyframeConfig<float> kfConfig{
        .keyframeConfig = {.value = 0},
        .timeType = KeyedTimeType::Interpolate
    };
    sut.AddKeyframe<float>("value", 0, kfConfig);
    sut.AddKeyframe<float>("value", 10, 10);

    float value = -1;
    sut.SetTrackBinding<float>("value", [&](auto _value) {
        value = _value;
    });

    EXPECT_EQ(0, value);

    auto track = sut.FindTrack("value");
    ASSERT_NE(nullptr, track);

    track->SetPlayTime(5);
    EXPECT_EQ(5, value);

    track->SetPlayTime(10);
    EXPECT_EQ(10, value);
}
