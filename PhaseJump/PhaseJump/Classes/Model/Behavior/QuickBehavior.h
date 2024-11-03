#pragma once

#include "BehaviorNode.h"
#include "ParallelBehavior.h"
#include "Timeline.h"

// /23
namespace PJ {
    class WorldNode;

    /// Tool to quickly build a behavior tree
    class QuickBehavior {
    public:
        using This = QuickBehavior;

        using BuildTimelineFunc = std::function<UP<Timeline>()>;
        using OnRunFunc = BehaviorNode::Func;

    protected:
        BehaviorNode& root;
        WorldNode& node;
        VectorList<BehaviorNode*> behaviors;
        bool _diagnose{};

    public:
        QuickBehavior(BehaviorNode& root, WorldNode& node);

        /// @return Returns the top behavior, if any
        BehaviorNode* TopBehavior() const;

        /// Pops the top behavior
        This& Pop(int count = 1);

        template <class Behavior>
        This& And(UP<Behavior>& behavior, String name) {
            GUARDR(behavior, *this)
            behavior->name = name;
            behavior->_diagnose.Enable(_diagnose);

            BehaviorNode* topBehavior = TopBehavior();
            auto behaviorPtr = behavior.get();

            if (topBehavior) {
                topBehavior->Add(behavior);
            }

            behaviors.push_back(behaviorPtr);

            return *this;
        }

        This& With(UP<BehaviorNode>& behavior, String name) {
            GUARDR(behavior, *this)
            behavior->name = name;
            behavior->_diagnose.Enable(_diagnose);

            TopBehavior()->Add(behavior);

            return *this;
        }

        /// Modify the last created behavior of the specified type
        template <class Type>
        This& ModifyLatest(std::function<void(Type& behavior)> func) {
            for (auto i = behaviors.crbegin(); i != behaviors.crend(); i++) {
                auto concrete = dynamic_cast<Type*>(*i);
                GUARD_CONTINUE(concrete)

                func(*concrete);
                break;
            };

            return *this;
        }

        /// Modify the last created behavior of any type
        This& ModifyLatestAny(std::function<void(BehaviorNode& behavior)> func) {
            for (auto i = behaviors.crbegin(); i != behaviors.crend(); i++) {
                func(**i);
                break;
            };

            return *this;
        }

        /// Turns diagnose flag on/off for upcoming nodes
        This& SetDiagnose(bool value) {
            _diagnose = value;
            return *this;
        }

        /// Turns on diagnose flag if we're in DEBUG mode
        This& DiagnoseIfDebug() {
#ifdef DEBUG
            return SetDiagnose(true);
#else
            return SetDiagnose(false);
#endif
        }

        /// Adds a random switch behavior and pushes it onto the behavior stack
        This& AndRandom(String name = {});

        /// Adds a random switch behavior with weights and pushes it onto the behavior stack
        This& AndRandomWeighted(String name, VectorList<float> const& weights);

        /// Adds a repeat behavior and pushes it onto the behavior stack
        This& AndRepeat(String name, int count);

        /// Adds a sequence behavior and pushes it onto the behavior stack
        This& AndSequence(String name = {});

        /// Adds a sequence behavior and pushes it onto the behavior stack
        This& AndUniformTimedSequence(float duration, VectorList<OnRunFunc> const& onRunFuncs);

        /// Adds a delay behavior and pushes it onto the behavior stack
        This& AndDelay(String name, float delay);

        /// Adds a delay behavior and pushes it onto the behavior stack
        This& AndDelay(String name, std::function<float()> delayValueFunc);

        /// Adds a parallel behavior and pushes it onto the behavior stack
        This& AndParallel(
            String name, int successThreshold = ParallelBehavior::AllThreshold,
            int failureThreshold = ParallelBehavior::AllThreshold
        );

        /// Adds a timed behavior and pushes it onto the behavior stack
        This& AndTimed(String name, float duration);

        /// Adds a timed behavior and pushes it onto the behavior stack
        This& AndTimed(String name, std::function<float()> durationValueFunc);

        /// Adds a timeline behavior as a child of the current node
        This& Timeline(String name, BuildTimelineFunc buildTimelineFunc);

        /// Adds an action behavior as a child of the current node
        This& Action(String name, OnRunFunc onRunFunc);

        /// Sets the state in the animation controller and finishes when the animation does
        This& AnimationState(String value);

        /// Sends an input the animation controller, and finishes when the animation does
        This& AnimationInput(String value);

        /// Sets the state in the components controller and finishes immediately
        This& ComponentsState(String value);

        /// Adds a delay behavior as a child of the current node
        This& Delay(String name, float delay);

        /// Adds a delay behavior as a child of the current node
        This& Delay(String name, std::function<float()> delayValueFunc);

        // FUTURE: add behaviors as needed
    };
} // namespace PJ
