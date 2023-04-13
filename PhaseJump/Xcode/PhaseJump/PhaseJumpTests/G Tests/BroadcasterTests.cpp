#include "gtest/gtest.h"
#include "Broadcaster.h"
#include "SomeListener.h"

// TODO: Temp for build tests
#include "RateLimiter.h"
#include "Alignment.h"
#include "AnimationCycleTimer.h"
#include "CirclePath.h"
#include "PublishedValue.h"

#include "SomeGraphNode.h"

#include "Axis.h"
#include "Interpolator.h"
#include "SomeInterpolate.h"
#include "Alignment.h"
#include "SomePath.h"
#include "LinePath.h"
#include "TransformTimer.h"
#include "SomeFocusCoordinator.h"
#include "Angle.h"
#include "Color32.h"
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

#include <memory>

// TODO: temp code
#include "SomeAligner.h"
#include "IntClamp.h"
#include "SomeGraphNode.h"
#include "AcyclicGraphNode.h"
#include "World.h"
#include "SomeStateMachine.h"
#include "StateMachine.h"
#include "PublishedValue.h"
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

using namespace PJ;
using namespace std;

namespace BroadcasterTests {
    class TestListener : public SomeListener {
    public:
        int listenCount = 0;
        String lastMessage;

        void OnEvent(EventPtr event) override {
            listenCount++;
            lastMessage = event->id;
        }
    };
}

using namespace BroadcasterTests;

TEST(Broadcaster, Factory) {
    Weight<float> weight(1.0f);

    Broadcaster sut;
    auto listener = make_shared<TestListener>();
    auto listener2 = make_shared<TestListener>();
    sut.AddListener(listener);
    EXPECT_EQ(1, sut.listeners.size());
    sut.RemoveListener(listener);
    EXPECT_EQ(0, sut.listeners.size());

    sut.AddListener(listener);
    sut.AddListener(listener2);
    sut.Broadcast(make_shared<Event>("hello"));
    EXPECT_EQ("hello", listener->lastMessage);
    EXPECT_EQ("hello", listener2->lastMessage);

    sut.RemoveListener(listener);
    sut.Broadcast(make_shared<Event>("goodbye"));
    EXPECT_EQ("hello", listener->lastMessage);
    EXPECT_EQ("goodbye", listener2->lastMessage);

    sut.Clear();
    EXPECT_EQ(0, sut.listeners.size());
}
