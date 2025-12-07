#include "RepeatBehavior.h"

using namespace std;
using namespace PJ;

RepeatBehavior::RepeatBehavior(RepeatValueFunc repeatValueFunc) :
    repeatValueFunc(repeatValueFunc) {}

RepeatBehavior::RepeatBehavior(int repeat) :
    RepeatBehavior([=]() { return repeat; }) {}

void RepeatBehavior::OnTargetFinish(BehaviorNode& target) {
    Base::OnTargetFinish(target);

    repeats--;
    if (repeats > 0) {
        target.Reset();
    } else {
        Finish();
    }
}

BehaviorNode* RepeatBehavior::Target() {
    GUARDR(repeats > 0, {})
    return Base::Target();
}

void RepeatBehavior::OnRun() {
    Base::OnRun();

    GUARD(repeatValueFunc)
    repeats = repeatValueFunc();
    repeatMax = repeats;

    GUARD(repeats <= 0)
    finishState = BehaviorState::Failure;
    Finish();
}
