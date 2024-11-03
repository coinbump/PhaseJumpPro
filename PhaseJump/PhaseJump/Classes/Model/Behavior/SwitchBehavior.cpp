#include "SwitchBehavior.h"

using namespace std;
using namespace PJ;

SwitchBehavior::SwitchBehavior(ChooseFunc chooseFunc) :
    chooseFunc(chooseFunc) {}

BehaviorNode* SwitchBehavior::Target() {
    return target;
}

void SwitchBehavior::OnRun() {
    Base::OnRun();

    target = nullptr;

    if (0 == tree.ChildCount()) {
        finishState = BehaviorState::Failure;
        Finish();
    }

    GUARD(chooseFunc)
    auto choice = chooseFunc(*this);
    GUARD(choice >= 0 && choice < tree.ChildCount())

    target = tree.Children()[choice].get();
}

void SwitchBehavior::OnTargetFinish(BehaviorNode& target) {
    Base::OnTargetFinish(target);

    // Run one behavior at a time
    Finish();
}
