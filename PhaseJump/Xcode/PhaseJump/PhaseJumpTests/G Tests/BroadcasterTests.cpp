#include "gtest/gtest.h"
#include "Broadcaster.h"
#include "SomeListener.h"

// TODO: Temp for build tests
#include "RateLimiter.h"
#include "Alignment.h"
#include "AnimationCycleTimer.h"
#include "CirclePath.h"

#include "SomeGraphNode.h"

#include "Axis.h"
#include "Interpolator.h"
#include "SomeInterpolate.h"
#include "Alignment.h"
#include "SomePath.h"
#include "LinePath.h"
#include "TransformTimer.h"
#include "Angle.h"
#include "RGBAColor.h"
#include "SomePosition.h"
#include "Polygon.h"
#include "Node2D.h"
#include "SwitchHandler.h"
#include "SomeEffect.h"
#include "SomeAnimatedEffect.h"
#include "SomeLayout.h"
#include "SomeLayout2D.h"
#include "HFlow.h"
#include "VFlow.h"
#include "SomeGoal.h"
#include "GoalEvents.h"
#include "Signal.h"
#include "SomeReceptor.h"
#include "SomeRandom.h"
#include "FixedRandom.h"
#include "StandardRandom.h"
#include "Weight.h"
#include "WeightedRandomChoice.h"
#include "LinePathLayout.h"
#include "BezierPathLayout.h"
#include "CirclePathLayout2D.h"
#include "SDLImGuiRenderContext.h"
#include "CenterPolyMeshBuilder.h"
#include "ArcMeshBuilder.h"
#include "PhaseJump.h"
#include <memory>

// TODO: temp code
#include "SomeAligner.h"
#include "IntClamp.h"
#include "SomeGraphNode.h"
#include "AcyclicGraphNode.h"
#include "World.h"
#include "SomeStateMachine.h"
#include "StateMachine.h"
#include "CycleAnimator.h"
#include "SomeKeyframeInterpolatorFactory.h"
#include "SomeKeyframe.h"
#include "ValueKeyframe.h"
#include "ActionKeyframe.h"
#include "Switchable.h"
#include "Animator.h"
#include "TimedSequence.h"
#include "SomeRandomChoice.h"
#include "BroadcastTimer.h"
#include "GoStateMachine.h"
#include "SomeRunner.h"
#include "Valve.h"
#include "GoCore.h"
#include "SomeNode.h"
#include "SomeStateHandler.h"
#include "SomeEventHandler.h"
#include "ToggleButtonControl.h"
#include "SomeDropTarget.h"
#include "GridViewItem.h"
#include "HStack.h"
#include "PadView.h"
#include "ZStack.h"
#include "VStack.h"
#include "SomeDragGestureHandler2D.h"
#include "HoverHandler.h"

using namespace PJ;
using namespace std;

namespace BroadcasterTests {
    class TestListener : public SomeListener {
    public:
        int listenCount = 0;
        String lastMessage;

        void OnEvent(SPC<Event> event) override {
            listenCount++;
            lastMessage = event->id;
        }
    };
}

using namespace BroadcasterTests;

TEST(Broadcaster, Factory) {
    Weight<float> weight(1.0f);

    Broadcaster sut;
    auto listener = MAKE<TestListener>();
    auto listener2 = MAKE<TestListener>();
    sut.AddListener(listener);
    EXPECT_EQ(1, sut.listeners.size());
    sut.RemoveListener(listener);
    EXPECT_EQ(0, sut.listeners.size());

    sut.AddListener(listener);
    sut.AddListener(listener2);
    sut.Broadcast(MAKE<Event>("hello"));
    EXPECT_EQ("hello", listener->lastMessage);
    EXPECT_EQ("hello", listener2->lastMessage);

    sut.RemoveListener(listener);
    sut.Broadcast(MAKE<Event>("goodbye"));
    EXPECT_EQ("hello", listener->lastMessage);
    EXPECT_EQ("goodbye", listener2->lastMessage);

    sut.Clear();
    EXPECT_EQ(0, sut.listeners.size());
}
