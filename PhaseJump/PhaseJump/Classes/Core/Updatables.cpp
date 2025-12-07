#include "Updatables.h"

using namespace std;
using namespace PJ;

SP<SomeUpdatable> Updatables::AddTimed(float duration, SimpleOnUpdateFunc onUpdateFunc) {
    auto result = MakeTimedFunc(duration, onUpdateFunc);
    return Add(result);
}

SP<SomeUpdatable> Updatables::AddDelay(float delay, SP<SomeUpdatable> value) {
    return Add(MakeDelayFunc(delay, value));
}

SP<SomeUpdatable> Updatables::AddDelay(float delay, SimpleOnUpdateFunc onUpdateFunc) {
    auto result = MakeDelayFunc(delay, onUpdateFunc);
    return Add(result);
}

SP<SomeUpdatable> Updatables::AddSpeed(float speed, SimpleOnUpdateFunc onUpdateFunc) {
    auto result = MakeSpeedFunc(speed, onUpdateFunc);
    return Add(result);
}

Updatables::SimpleOnUpdateFunc
Updatables::MakeTimedFunc(float duration, SimpleOnUpdateFunc onUpdateFunc) {
    return [=, state = 0.0f](TimeSlice time) mutable {
        GUARDR(state < duration, FinishType::Finish)
        state += time.delta;
        auto updateResult = onUpdateFunc(time);
        return state < duration ? updateResult : FinishType::Finish;
    };
}

Updatables::SimpleOnUpdateFunc
Updatables::MakeDelayFunc(float delay, SimpleOnUpdateFunc onUpdateFunc) {
    return [=](TimeSlice time) mutable {
        if (delay > 0) {
            delay -= time.delta;
            return FinishType::Continue;
        }
        return onUpdateFunc(time);
    };
}

Updatables::SimpleOnUpdateFunc Updatables::MakeDelayFunc(float delay, SP<SomeUpdatable> updatable) {
    return [=](TimeSlice time) mutable {
        if (delay > 0) {
            delay -= time.delta;
            return FinishType::Continue;
        }
        return updatable->OnUpdate(time);
    };
}

Updatables::SimpleOnUpdateFunc
Updatables::MakeSpeedFunc(float speed, SimpleOnUpdateFunc onUpdateFunc) {
    return [=](TimeSlice time) { return onUpdateFunc(time * speed); };
}

FinishType Updatables::OnUpdate(TimeSlice time) {
    GUARDR(!IsEmpty(list), FinishType::Continue)

    UpdatableList freshList;

    // Careful: updatables might be added while we are running the updatables loop
    auto iter = list;
    std::for_each(iter.begin(), iter.end(), [&](SP<SomeUpdatable>& u) {
        GUARD(!u->IsFinished())

        u->OnUpdate(time);
    });

    RemoveIf(list, [](auto& element) { return element->IsFinished(); });

    return FinishType::Continue;
}

/// Adds an updatable func that can finish
SP<SomeUpdatable> Updatables::Add(SimpleOnUpdateFunc onUpdateFunc) {
    auto result = MAKE<Updatable>([=](auto&, TimeSlice time) { return onUpdateFunc(time); });
    Add(result);

    return result;
}

/// Adds an updatable func that will continue running
SP<SomeUpdatable> Updatables::AddContinue(ContinueOnUpdateFunc onUpdateFunc) {
    auto result = MAKE<Updatable>([=](auto&, TimeSlice time) {
        onUpdateFunc(time);
        return FinishType::Continue;
    });
    Add(result);

    return result;
}

void Updatables::RemoveAll() {
    list.clear();
}
