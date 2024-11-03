#include "SequenceBehavior.h"

using namespace std;
using namespace PJ;

BehaviorNode* SequenceBehavior::Target() {
    GUARDR(targetIndex >= 0 && targetIndex < tree.ChildCount(), {})

    return tree.Children()[targetIndex].get();
}

void SequenceBehavior::OnTargetFinish(BehaviorNode& target) {
    Base::OnTargetFinish(target);

    switch (target.State()) {
    case BehaviorState::Failure:
        finishState = BehaviorState::Failure;
        Finish();
        break;
    case BehaviorState::Success:
        targetIndex++;
        if (targetIndex >= tree.ChildCount()) {
            finishState = BehaviorState::Success;
            Finish();
        }
        break;
    default:
        break;
    }
}

void SequenceBehavior::OnRun() {
    Base::OnRun();

    targetIndex = 0;

    if (targetIndex >= tree.ChildCount()) {
        finishState = BehaviorState::Failure;
        Finish();
    }
}
