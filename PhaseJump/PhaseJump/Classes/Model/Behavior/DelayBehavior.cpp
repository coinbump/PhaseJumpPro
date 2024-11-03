#include "DelayBehavior.h"

using namespace std;
using namespace PJ;

DelayBehavior::DelayBehavior(std::function<float()> delayValueFunc) :
    delayValueFunc(delayValueFunc) {}

DelayBehavior::DelayBehavior(float delay) :
    DelayBehavior([=]() { return delay; }) {}

BehaviorNode* DelayBehavior::Target() {
    GUARDR(delay <= 0, {})
    return Base::Target();
}

void DelayBehavior::OnRun() {
    Base::OnRun();

    GUARD(delayValueFunc)
    delay = delayValueFunc();

    onUpdateFunc = [](auto& updatable, auto time) {
        This& behavior = *(static_cast<This*>(&updatable));

        GUARDR(behavior.delay > 0, FinishType::Continue)
        behavior.delay -= time.delta;
        GUARDR(behavior.delay <= 0, FinishType::Continue)

        // Move to the child node after the delay
        // If there are no target nodes available, finish
        return behavior.Target() ? FinishType::Continue : FinishType::Finish;
    };

    GUARD(delay <= 0)
    finishState = BehaviorState::Failure;
    Finish();
}
