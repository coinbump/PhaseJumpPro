#include "UpdatableBehavior.h"

using namespace std;
using namespace PJ;

UpdatableBehavior::UpdatableBehavior(BuildFunc buildFunc) :
    buildFunc(buildFunc) {
    onUpdateFunc = [this](auto& _updatable, auto time) {
        GUARDR(updatable, FinishType::Finish);

        updatable->OnUpdate(time);
        return updatable->GetFinishType();
    };
}

void UpdatableBehavior::OnRun() {
    Base::OnRun();

    if (buildFunc) {
        updatable = buildFunc(*this);
    } else {
        finishState = BehaviorState::Failure;
        Finish();
    }
}
