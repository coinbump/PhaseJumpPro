#include "Updatables.h"

using namespace std;
using namespace PJ;

SP<Updatable> Updatables::AddTimed(float duration, OnUpdateFunc onUpdateFunc) {
    auto result = MakeTimedFunc(duration, onUpdateFunc);
    return Add(result);
}

SP<Updatable> Updatables::AddDelay(float delay, OnUpdateFunc onUpdateFunc) {
    auto result = MakeDelayFunc(delay, onUpdateFunc);
    return Add(result);
}

SP<Updatable> Updatables::AddSpeed(float speed, OnUpdateFunc onUpdateFunc) {
    auto result = MakeSpeedFunc(speed, onUpdateFunc);
    return Add(result);
}

Updatables::OnUpdateFunc Updatables::MakeTimedFunc(float duration, OnUpdateFunc onUpdateFunc) {
    return [=, state = 0.0f](TimeSlice time) mutable {
        GUARDR(state < duration, FinishType::Finish)
        state += time.delta;
        return onUpdateFunc(time);
    };
}

Updatables::OnUpdateFunc Updatables::MakeDelayFunc(float delay, OnUpdateFunc onUpdateFunc) {
    return [=](TimeSlice time) mutable {
        if (delay > 0) {
            delay -= time.delta;
            return FinishType::Continue;
        }
        return onUpdateFunc(time);
    };
}

Updatables::OnUpdateFunc Updatables::MakeSpeedFunc(float speed, OnUpdateFunc onUpdateFunc) {
    return [=](TimeSlice time) { return onUpdateFunc(time * speed); };
}
