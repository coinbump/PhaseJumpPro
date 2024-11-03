#include "BehaviorNode.h"
#include "CollectionUtils.h"

using namespace std;
using namespace PJ;

using This = BehaviorNode;

BehaviorNode::BehaviorNode(String _name, RunType runType) :
    tree(*this),
    name(_name) {
    switch (runType) {
    case RunType::Repeat:
        onAllChildrenFinishFunc = [](auto& behavior) { behavior.Reset(); };
        break;
    default:
        onAllChildrenFinishFunc = [](auto& behavior) { behavior.Finish(); };
        break;
    }

    onFinishFunc = [this](auto& updatable) {
        if (name.length() > 0) {
            PJ::LogIf(_diagnose, "Behavior- OnFinish: ", name);
        }

        states.SetState(finishState);
    };

    states.SetOnStateChangeFunc([this](auto& sm) {
        switch (sm.State()) {
        case BehaviorState::Initial:
            SetIsFinished(false);
            break;
        case BehaviorState::Run:
            OnRun();
            break;
        default:
            break;
        }
    });
}

void BehaviorNode::OnTargetFinish(BehaviorNode& target) {
    GUARD(onTargetFinishFunc);
    onTargetFinishFunc(*this, target);
}

This* BehaviorNode::Target() {
    GUARDR(tree.ChildCount() > 0, nullptr)
    return tree.Children()[0].get();
}

VectorList<BehaviorNode*> BehaviorNode::Targets() {
    auto target = Target();
    GUARDR(target, {})
    return { target };
}

void BehaviorNode::OnUpdate(TimeSlice time) {
    GUARD(!IsFinished())

    // Kickstart run if needed, which sets initial behavior states
    states.SetState(BehaviorState::Run);

    // Check if starting run failed
    GUARD(!IsFinished())

    // Target is determined before the update (Example: delay, timer)
    auto targets = Targets();

    Base::OnUpdate(time);

    VectorList<BehaviorNode*> unfinishedTargets;

    for (auto& target : targets) {
        GUARD_CONTINUE(!target->IsFinished())
        target->OnUpdate(time);

        unfinishedTargets.push_back(target);
    }

    if (!IsFinished()) {
        for (auto& target : unfinishedTargets) {
            GUARD_CONTINUE(target->IsFinished())
            OnTargetFinish(*target);
        }
    }

    // Finishing a target might finish the parent
    // If parent is finished, stop running child nodes
    // FUTURE: FinishDeep if needed (for vis dev)
    if (IsFinished()) {
        for (auto& node : tree.Children()) {
            GUARD_CONTINUE(node->IsRunning())
            node->Finish();

            // Don't call OnTargetFinish, parent behavior is finished
        }
    }

    GUARD(tree.ChildCount() > 0)

    auto firstUnfinishedChild =
        std::find_if(tree.Children().begin(), tree.Children().end(), [](auto& child) {
            return !child->IsFinished();
        });
    GUARD(firstUnfinishedChild == tree.Children().end())

    GUARD(onAllChildrenFinishFunc)
    onAllChildrenFinishFunc(*this);
}

void BehaviorNode::OnRun() {
    if (name.length() > 0) {
        PJ::LogIf(_diagnose, "Behavior- OnRun: ", name);
    }

    // Restart all children
    for (auto& child : tree.Children()) {
        child->Reset();
    }

    GUARD(onRunFunc)
    onRunFunc(*this);
}

int BehaviorNode::ChildSuccessCount() const {
    int result = 0;

    for (auto& child : tree.Children()) {
        if (child->State() == BehaviorState::Success) {
            result++;
        }
    }

    return result;
}

int BehaviorNode::ChildFailureCount() const {
    int result = 0;

    for (auto& child : tree.Children()) {
        if (child->State() == BehaviorState::Failure) {
            result++;
        }
    }

    return result;
}
