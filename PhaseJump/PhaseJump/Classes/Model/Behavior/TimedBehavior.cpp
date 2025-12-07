#include "TimedBehavior.h"

using namespace std;
using namespace PJ;

TimedBehavior::TimedBehavior(TimeValueFunc timeValueFunc) :
    timeValueFunc(timeValueFunc) {}

TimedBehavior::TimedBehavior(float time) :
    timeValueFunc([=]() { return time; }) {}

void TimedBehavior::OnRun() {
    Base::OnRun();

    GUARD(timeValueFunc);
    time = timeValueFunc();
    timeMax = time;

    updatable.onUpdateFunc = [this](auto& updatable, auto time) {
        This& behavior = *this;
        behavior.time -= time.delta;

        return behavior.time > 0 ? FinishType::Continue : FinishType::Finish;
    };

    GUARD(time <= 0)
    finishState = BehaviorState::Failure;
    Finish();
}
