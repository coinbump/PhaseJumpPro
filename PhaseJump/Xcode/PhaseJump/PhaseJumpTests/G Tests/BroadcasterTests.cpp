#include "gtest/gtest.h"
#include "Broadcaster.h"
#include "SomeListener.h"

#include "Function.h"
#include "Environment.h"
#include "RateLimiter.h"
#include "Alignment.h"
#include "AnimationCycleTimer.h"
#include "CirclePath.h"
#include "InputTriggerMap.h"
#include "Utils.h"

#include "SomeGraphNode.h"

#include "Axis.h"
#include "InterpolateFunc.h"
#include "EaseFunc.h"
#include "Alignment.h"
#include "SomePath.h"
#include "LinePath.h"
#include "Angle.h"
#include "RGBAColor.h"
#include "SomePosition.h"
#include "Polygon.h"
#include "Node2D.h"
#include "SwitchHandler.h"
#include "SomeEffect.h"
#include "SomeLayout.h"
#include "SomeLayout2D.h"
#include "HFlow.h"
#include "VFlow.h"
#include "SomeGoal.h"
#include "GoalEvents.h"
#include "Signal.h"
#include "Receptor.h"
#include "SomeRandom.h"
#include "MockRandom.h"
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

#include "SomeProducer.h"
#include "CirclePositioner2D.h"
#include "RectPositioner2D.h"
#include "SomeAligner.h"
#include "TransformFunc.h"
#include "SomeGraphNode.h"
#include "AcyclicGraphNode.h"
#include "World.h"
#include "SomeStateMachine.h"
#include "StateMachine.h"
#include "Agent.h"
#include "AgentSystem.h"
#include "SomeKeyframe.h"
#include "Keyframe.h"
#include "Switchable.h"
#include "Animator.h"
#include "SomeRandomChoice.h"
#include "SomeAttribute.h"
#include "TimedStateDriver.h"
#include "TimeTrack.h"
#include "Runner.h"
#include "MatrixPiece.h"
#include "Valve.h"
#include "SomeNode.h"
#include "TimeTrack.h"
#include "ToggleButtonControl.h"
#include "SomeDropTarget.h"
#include "GridViewItem.h"
#include "HStack.h"
#include "PadView.h"
#include "ZStack.h"
#include "VStack.h"
#include "SomeDragGestureHandler2D.h"

using namespace PJ;
using namespace std;

namespace BroadcasterTests {
    class TestListener : public SomeListener {
    public:
        int listenCount = 0;
        String lastMessage;

        void OnEvent(SP<SomeSignal> event) override {
            listenCount++;

            auto standardEvent = As<Event<StandardEventCore>>(event.get());
            GUARD(standardEvent);
            lastMessage = standardEvent->core.id;
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
    sut.Broadcast(MAKE<Event<StandardEventCore>>(StandardEventCore("hello")));
    EXPECT_EQ("hello", listener->lastMessage);
    EXPECT_EQ("hello", listener2->lastMessage);

    sut.RemoveListener(listener);
    sut.Broadcast(MAKE<Event<StandardEventCore>>(StandardEventCore("goodbye")));
    EXPECT_EQ("hello", listener->lastMessage);
    EXPECT_EQ("goodbye", listener2->lastMessage);

    sut.Clear();
    EXPECT_EQ(0, sut.listeners.size());
}
