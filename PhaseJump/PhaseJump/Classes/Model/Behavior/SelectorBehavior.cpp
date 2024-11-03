#include "SelectorBehavior.h"

using namespace std;
using namespace PJ;

BehaviorNode* SelectorBehavior::Target() {
    GUARDR(targetIndex >= 0 && targetIndex < tree.ChildCount(), {})

    return tree.Children()[targetIndex].get();
}

void SelectorBehavior::OnTargetFinish(BehaviorNode& target) {
    Base::OnTargetFinish(target);

    targetIndex++;

    switch (target.State()) {
    case BehaviorState::Failure:
        if (targetIndex >= tree.ChildCount()) {
            finishState = BehaviorState::Failure;
            Finish();
        }
        break;
    case BehaviorState::Success:
        finishState = BehaviorState::Success;
        Finish();
        break;
    default:
        break;
    }
}

void SelectorBehavior::OnRun() {
    Base::OnRun();

    targetIndex = 0;

    if (targetIndex >= tree.ChildCount()) {
        finishState = BehaviorState::Failure;
        Finish();
    }
}
