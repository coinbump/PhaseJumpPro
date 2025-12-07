#include "gtest/gtest.h"
#include "Playable.h"
#include "PlayableTimer.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace PlayableTests {
    class TestControllerPlayable : public Playable
    {
    public:
        using Base = Playable;
        
        PlayableTimer timer;

        TestControllerPlayable() : timer(10, RunType::Once) {
            controller = &timer;
        }
    };
}

using namespace PlayableTests;

TEST(Playable, SetOnPlayStateChangeFunc) {
    int changeCount{};
    
    Playable sut(PlayState::Play);
    sut.SetOnPlayStateChangeFunc([&](auto &playable) {
        changeCount++;
    });
    
    EXPECT_EQ(changeCount, 1);
    sut.SetPlayState(PlayState::Pause);
    EXPECT_EQ(changeCount, 2);
}

TEST(Playable, SetPlayState) {
    Playable sut(PlayState::Play);
    EXPECT_EQ(PlayState::Play, sut.GetPlayState());
    
    sut.SetPlayState(PlayState::Pause);
    EXPECT_EQ(PlayState::Pause, sut.GetPlayState());
}

TEST(Playable, HasController) {
    TestControllerPlayable sut;
    EXPECT_TRUE(sut.HasController());
}

TEST(Playable, ControllerDuration) {
    TestControllerPlayable sut;
    EXPECT_EQ(10, sut.Duration());
}

TEST(Playable, ControllerSetDuration) {
    TestControllerPlayable sut;
    EXPECT_EQ(10, sut.Duration());
    sut.SetDuration(5);
    EXPECT_EQ(5, sut.Duration());
}

TEST(Playable, ControllerProgress) {
    TestControllerPlayable sut;
    EXPECT_EQ(0, sut.Progress());
    sut.SetProgress(0.5f);
    EXPECT_EQ(0.5f, sut.Progress());
    EXPECT_EQ(5, sut.PlayTime());
}

TEST(Playable, ControllerPlayTime) {
    TestControllerPlayable sut;
    EXPECT_EQ(0, sut.Progress());
    sut.SetPlayTime(5);
    EXPECT_EQ(0.5f, sut.Progress());
    EXPECT_EQ(5, sut.PlayTime());
}

TEST(Playable, ControllerPlayStates) {
    TestControllerPlayable sut;
    sut.Play();
    EXPECT_TRUE(sut.IsPlaying());
    EXPECT_FALSE(sut.IsPaused());
    sut.Pause();
    EXPECT_FALSE(sut.IsPlaying());
    EXPECT_TRUE(sut.IsPaused());
    sut.Stop();
}

TEST(Playable, ControllerOnPlayTimeChange) {
    int changeCount{};
    
    TestControllerPlayable sut;
    sut.SetOnPlayTimeChangeFunc([&](auto& playable) {
        changeCount++;
    });
    
    EXPECT_EQ(1, changeCount);
    
    sut.SetPlayTime(5);
    EXPECT_EQ(2, changeCount);
}

TEST(Playable, ControllerOnUpdate) {
    TestControllerPlayable sut;
    sut.Play();
    EXPECT_TRUE(sut.IsPlaying());
    sut.OnUpdate({3});
    EXPECT_EQ(3, sut.PlayTime());
}

TEST(Playable, ControllerIsFinished) {
    TestControllerPlayable sut;
    sut.Play();
    EXPECT_TRUE(sut.IsPlaying());
    sut.OnUpdate({3});
    EXPECT_FALSE(sut.IsFinished());
    sut.OnUpdate({10});
    EXPECT_TRUE(sut.IsFinished());
}

TEST(Playable, ControllerUpdateWhenPaused) {
    TestControllerPlayable sut;
    sut.Pause();
    sut.OnUpdate({12});
    EXPECT_FALSE(sut.IsFinished());
}

TEST(Playable, ControllerReset) {
    TestControllerPlayable sut;
    sut.Play();
    sut.OnUpdate({12});
    EXPECT_TRUE(sut.IsFinished());
    
    sut.Reset();
    EXPECT_EQ(0, sut.PlayTime());
    EXPECT_FALSE(sut.IsPaused());
    EXPECT_FALSE(sut.IsFinished());
}

TEST(Playable, OnUpdateWithControllerReturnsValue) {
    TestControllerPlayable sut;
    sut.Play();
    EXPECT_TRUE(sut.IsPlaying());
    EXPECT_EQ(FinishType::Continue, sut.OnUpdate({3}));
    EXPECT_EQ(FinishType::Finish, sut.OnUpdate({7}));
    EXPECT_EQ(FinishType::Finish, sut.OnUpdate({1}));
}
