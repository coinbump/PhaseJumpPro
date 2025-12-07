#include "UpdatableBehavior.h"

using namespace std;
using namespace PJ;

UpdatableBehavior::UpdatableBehavior(BuildFunc buildFunc) :
    buildFunc(buildFunc) {
    updatable.onUpdateFunc = [this](auto& _updatable, auto time) {
        GUARDR(childUpdatable, FinishType::Finish);

        return childUpdatable->OnUpdate(time);
    };
}

void UpdatableBehavior::OnRun() {
    Base::OnRun();

    if (buildFunc) {
        childUpdatable = buildFunc(*this);
    } else {
        finishState = BehaviorState::Failure;
        Finish();
    }
}
