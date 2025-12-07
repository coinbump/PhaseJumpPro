#include "QuickBehavior.h"
#include "ComponentsController.h"
#include "DelayBehavior.h"
#include "ParallelBehavior.h"
#include "RepeatBehavior.h"
#include "SequenceBehavior.h"
#include "SimpleAnimationController.h"
#include "SomeRenderer.h"
#include "StandardRandom.h"
#include "SwitchBehavior.h"
#include "TimedBehavior.h"
#include "UpdatableBehavior.h"
#include "WeightedRandomChoice.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

using This = QuickBehavior;

QuickBehavior::QuickBehavior(BehaviorNode& root, WorldNode& node) :
    root(root),
    node(node) {}

BehaviorNode* QuickBehavior::TopBehavior() const {
    GUARDR(!IsEmpty(behaviors), &root)
    return behaviors[behaviors.size() - 1];
}

This& QuickBehavior::Pop(int count) {
    while (count > 0) {
        GUARDR(!IsEmpty(behaviors), *this)
        behaviors.erase(behaviors.begin() + behaviors.size() - 1);

        count--;
    }
    return *this;
}

This& QuickBehavior::AndRandom(String name) {
    UP<BehaviorNode> behavior = NEW<SwitchBehavior>([](auto& behavior) {
        return StandardRandom().Choice((int)behavior.tree.ChildCount());
    });
    return And(behavior, name);
}

This& QuickBehavior::AndRandomWeighted(String name, VectorList<float> const& weights) {
    UP<BehaviorNode> behavior = NEW<SwitchBehavior>([weights](auto& behavior) {
        WeightedRandomChoice<int> weightedChoice;
        size_t index = 0;
        for (auto& weight : weights) {
            UP<SomeWeight<int>> choiceWeight = NEW<Weight<int>>(weight, index);
            weightedChoice.Add(choiceWeight);
            index++;
        }
        StandardRandom random;
        auto choice = weightedChoice.Choose(random);
        return choice ? *choice : 0;
    });
    return And(behavior, name);
}

This& QuickBehavior::AndRepeat(String name, int count) {
    UP<BehaviorNode> behavior = NEW<RepeatBehavior>(count);
    return And(behavior, name);
}

This& QuickBehavior::AndSequence(String name) {
    UP<BehaviorNode> behavior = NEW<SequenceBehavior>();
    return And(behavior, name);
}

This& QuickBehavior::Action(String name, OnRunFunc onRunFunc) {
    UP<BehaviorNode> behavior = NEW<BehaviorNode>();
    behavior->onRunFunc = [=](auto& behavior) {
        onRunFunc(behavior);
        behavior.Finish();
    };
    behavior->name = name;
    behavior->_diagnose.Enable(_diagnose);
    TopBehavior()->Add(behavior);

    return *this;
}

This& QuickBehavior::AndUniformTimedSequence(
    float duration, VectorList<OnRunFunc> const& onRunFuncs
) {
    AndSequence();

    for (size_t i = 0; i < onRunFuncs.size(); i++) {
        auto& onRunFunc = onRunFuncs[i];
        AndTimed("Uniform Sequence: " + MakeString(i), duration)
            .ModifyLatestAny([=](auto& behavior) { Override(behavior.onRunFunc, onRunFunc); })
            .Pop();
    }

    return *this;
}

This& QuickBehavior::AndDelay(String name, float delay) {
    return AndDelay(name, [=]() { return delay; });
}

This& QuickBehavior::AndDelay(String name, std::function<float()> delayValueFunc) {
    UP<BehaviorNode> behavior = NEW<DelayBehavior>(delayValueFunc);
    return And(behavior, name);
}

This& QuickBehavior::AndParallel(String name, int successThreshold, int failureThreshold) {
    UP<BehaviorNode> behavior = NEW<ParallelBehavior>(successThreshold, failureThreshold);
    return And(behavior, name);
}

This& QuickBehavior::AndTimed(String name, float duration) {
    return AndTimed(name, [=]() { return duration; });
}

This& QuickBehavior::AndTimed(String name, std::function<float()> durationValueFunc) {
    UP<BehaviorNode> behavior = NEW<TimedBehavior>(durationValueFunc);
    return And(behavior, name);
}

This& QuickBehavior::Timeline(String name, BuildTimelineFunc buildTimelineFunc) {
    GUARDR(buildTimelineFunc, *this);

    BehaviorNode* topBehavior = TopBehavior();
    GUARDR(topBehavior, *this);

    UP<BehaviorNode> behavior =
        NEW<UpdatableBehavior>([=](auto& behavior) { return buildTimelineFunc(); });
    return With(behavior, name);
}

This& QuickBehavior::AnimationState(String value) {
    UP<BehaviorNode> behavior = NEW<BehaviorNode>();
    auto& node = this->node;
    behavior->onRunFunc = [&node, value](auto& behavior) {
        auto simpleAnimationController = node.TypeComponent<SimpleAnimationController>();
        GUARD(simpleAnimationController);

        // We might be repeating the same animation state, so reset the animation
        simpleAnimationController->SetState(value, PublishValueCondition::Always);
    };
    behavior->updatable.onUpdateFunc = [&node](auto& updatable, auto time) {
        auto renderer = node.TypeComponent<SomeRenderer>();
        GUARDR(renderer, FinishType::Finish)

        return renderer->IsRenderFinished() ? FinishType::Finish : FinishType::Continue;
    };

    return With(behavior, "Animation State: " + value);
}

This& QuickBehavior::AnimationInput(String value) {
    UP<BehaviorNode> behavior = NEW<BehaviorNode>();
    auto& node = this->node;
    behavior->onRunFunc = [&node, value](auto& behavior) {
        auto simpleAnimationController = node.TypeComponent<SimpleAnimationController>();
        GUARD(simpleAnimationController);

        // Inputs don't reset the animation, so an input that leads to the existing state might
        // finish early. Always use AnimationState to set the initial state for a sequence
        simpleAnimationController->states.Input(value);
    };
    behavior->updatable.onUpdateFunc = [&node](auto& updatable, auto time) {
        auto renderer = node.TypeComponent<SomeRenderer>();
        GUARDR(renderer, FinishType::Finish)

        return renderer->IsRenderFinished() ? FinishType::Finish : FinishType::Continue;
    };

    return With(behavior, "Animation Input: " + value);
}

This& QuickBehavior::Delay(String name, float delay) {
    return Delay(name, [=]() { return delay; });
}

This& QuickBehavior::Delay(String name, std::function<float()> delayValueFunc) {
    UP<BehaviorNode> behavior = NEW<DelayBehavior>(delayValueFunc);
    return With(behavior, name);
}

This& QuickBehavior::ComponentsState(String value) {
    auto& node = this->node;

    return Action("Components State: " + value, [value, &node](auto& behavior) {
        auto componentsController = node.TypeComponent<ComponentsController>();
        GUARD(componentsController);

        componentsController->states.SetState(value);
    });
}
