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
    using TimeTrack = PJ::TimeTrack<float, EaseKeyframe<float>>;

    Timeline sut(10, AnimationCycleType::Once, Timeline::PlayType::TimelineDrives);
    EXPECT_EQ(10, sut.Duration());
    UP<Playable> track1 = std::make_unique<TimeTrack>(10);
    UP<Playable> track2 = std::make_unique<TimeTrack>(10);

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

TEST(Timeline, OnUpdateWhenTracksDrive)
{
    using TimeTrack = PJ::TimeTrack<float, EaseKeyframe<float>>;

    Timeline sut(10, AnimationCycleType::Once, Timeline::PlayType::TimelineDrives);
    EXPECT_EQ(10, sut.Duration());
    UP<Playable> track1 = std::make_unique<TimeTrack>(10, AnimationCycleType::Once);
    UP<Playable> track2 = std::make_unique<TimeTrack>(10, AnimationCycleType::Once);

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
    EXPECT_EQ(15, value1);
    EXPECT_EQ(150, value2);
    EXPECT_FALSE(track1Ptr->IsFinished());
    EXPECT_FALSE(track2Ptr->IsFinished());
    EXPECT_FALSE(sut.IsFinished());

    sut.OnUpdate({5});
    EXPECT_EQ(20, value1);
    EXPECT_EQ(200, value2);
    EXPECT_TRUE(track1Ptr->IsFinished());
    EXPECT_TRUE(track2Ptr->IsFinished());
    EXPECT_TRUE(sut.IsFinished());
}
