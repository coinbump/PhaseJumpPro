#include "ButtonTracker.h"
#include "gtest/gtest.h"

using namespace std;
using namespace PJ;

namespace ButtonTrackerTests {
    /// Recording callbacks so tests can assert how many times each fired.
    struct Recorder {
        int pressCount = 0;
        int stateChangeCount = 0;

        void Setup(ButtonTracker& sut) {
            sut.onPressFunc = [this]() { ++pressCount; };
            sut.onStateChangeFunc = [this]() { ++stateChangeCount; };
        }
    };
} // namespace ButtonTrackerTests

using namespace ButtonTrackerTests;

// MARK: - Initial state

TEST(ButtonTracker, InitialState) {
    ButtonTracker sut;

    EXPECT_FALSE(sut.IsTracking());
    EXPECT_FALSE(sut.IsPressed());
    EXPECT_EQ(ButtonTracker::TrackType::Track, sut.trackType);
}

// MARK: - Track mode (default)

TEST(ButtonTracker, TrackMode_PointerDown_BeginsTracking_DoesNotFirePress) {
    ButtonTracker sut;
    Recorder recorder;
    recorder.Setup(sut);

    sut.OnPointerDown();

    EXPECT_TRUE(sut.IsTracking());
    EXPECT_TRUE(sut.IsPressed());
    EXPECT_EQ(0, recorder.pressCount);
    EXPECT_EQ(1, recorder.stateChangeCount);
}

TEST(ButtonTracker, TrackMode_DownThenUp_FiresPressOnce) {
    ButtonTracker sut;
    Recorder recorder;
    recorder.Setup(sut);

    sut.OnPointerDown();
    sut.OnPointerUp();

    EXPECT_FALSE(sut.IsTracking());
    EXPECT_FALSE(sut.IsPressed());
    EXPECT_EQ(1, recorder.pressCount);

    EXPECT_EQ(2, recorder.stateChangeCount);
}

TEST(ButtonTracker, TrackMode_DownExitUp_DoesNotFirePress) {
    ButtonTracker sut;
    Recorder recorder;
    recorder.Setup(sut);

    sut.OnPointerDown();
    sut.OnPointerExit();
    sut.OnPointerUp();

    EXPECT_FALSE(sut.IsTracking());
    EXPECT_FALSE(sut.IsPressed());
    EXPECT_EQ(0, recorder.pressCount);
}

TEST(ButtonTracker, TrackMode_DownExitEnterUp_FiresPress) {
    ButtonTracker sut;
    Recorder recorder;
    recorder.Setup(sut);

    sut.OnPointerDown();
    sut.OnPointerExit();
    sut.OnPointerEnter();
    sut.OnPointerUp();

    EXPECT_EQ(1, recorder.pressCount);
}

TEST(ButtonTracker, TrackMode_EnterWithoutDown_Ignored) {
    ButtonTracker sut;
    Recorder recorder;
    recorder.Setup(sut);

    sut.OnPointerEnter();

    EXPECT_FALSE(sut.IsTracking());
    EXPECT_FALSE(sut.IsPressed());
    EXPECT_EQ(0, recorder.pressCount);
    EXPECT_EQ(0, recorder.stateChangeCount);
}

TEST(ButtonTracker, TrackMode_ExitWithoutDown_Ignored) {
    ButtonTracker sut;
    Recorder recorder;
    recorder.Setup(sut);

    sut.OnPointerExit();

    EXPECT_FALSE(sut.IsTracking());
    EXPECT_FALSE(sut.IsPressed());
    EXPECT_EQ(0, recorder.stateChangeCount);
}

TEST(ButtonTracker, TrackMode_UpWithoutDown_Ignored) {
    ButtonTracker sut;
    Recorder recorder;
    recorder.Setup(sut);

    sut.OnPointerUp();

    EXPECT_FALSE(sut.IsTracking());
    EXPECT_FALSE(sut.IsPressed());
    EXPECT_EQ(0, recorder.pressCount);
    EXPECT_EQ(0, recorder.stateChangeCount);
}

TEST(ButtonTracker, TrackMode_RedundantEnterWhilePressed_NoExtraStateChange) {
    ButtonTracker sut;
    Recorder recorder;
    recorder.Setup(sut);

    sut.OnPointerDown();
    int baseline = recorder.stateChangeCount;
    sut.OnPointerEnter();

    EXPECT_TRUE(sut.IsPressed());
    EXPECT_EQ(baseline, recorder.stateChangeCount);
}

TEST(ButtonTracker, TrackMode_DoubleExit_SecondIsNoOp) {
    ButtonTracker sut;
    Recorder recorder;
    recorder.Setup(sut);

    sut.OnPointerDown();
    sut.OnPointerExit();
    sut.OnPointerExit();

    EXPECT_EQ(2, recorder.stateChangeCount);
    EXPECT_TRUE(sut.IsTracking());
}

TEST(ButtonTracker, TrackMode_MultiplePressCycles_FireIndependently) {
    ButtonTracker sut;
    Recorder recorder;
    recorder.Setup(sut);

    sut.OnPointerDown();
    sut.OnPointerUp();
    sut.OnPointerDown();
    sut.OnPointerUp();
    sut.OnPointerDown();
    sut.OnPointerUp();

    EXPECT_EQ(3, recorder.pressCount);
    EXPECT_FALSE(sut.IsTracking());
    EXPECT_FALSE(sut.IsPressed());
}

// MARK: - Immediate mode

TEST(ButtonTracker, ImmediateMode_PointerDown_FiresPressImmediately) {
    ButtonTracker sut;
    sut.trackType = ButtonTracker::TrackType::Immediate;
    Recorder recorder;
    recorder.Setup(sut);

    sut.OnPointerDown();

    EXPECT_EQ(1, recorder.pressCount);
    EXPECT_FALSE(sut.IsTracking());
    EXPECT_FALSE(sut.IsPressed());

    EXPECT_EQ(0, recorder.stateChangeCount);
}

TEST(ButtonTracker, ImmediateMode_PointerUpAfterDown_NoEffect) {
    ButtonTracker sut;
    sut.trackType = ButtonTracker::TrackType::Immediate;
    Recorder recorder;
    recorder.Setup(sut);

    sut.OnPointerDown();
    sut.OnPointerUp();

    EXPECT_EQ(1, recorder.pressCount);
}

TEST(ButtonTracker, NoCallbacks_FullCycle_DoesNotCrash) {
    ButtonTracker sut;

    sut.OnPointerDown();
    sut.OnPointerExit();
    sut.OnPointerEnter();
    sut.OnPointerUp();

    EXPECT_FALSE(sut.IsTracking());
    EXPECT_FALSE(sut.IsPressed());
}

TEST(ButtonTracker, NoCallbacks_ImmediateDown_DoesNotCrash) {
    ButtonTracker sut;
    sut.trackType = ButtonTracker::TrackType::Immediate;

    sut.OnPointerDown();

    EXPECT_FALSE(sut.IsPressed());
}
