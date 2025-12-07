#include "AnimationCycleTimer.h"

using namespace std;
using namespace PJ;

AnimationCycleTimer::AnimationCycleTimer(float duration, AnimationCycleType cycleType) :
    timer(duration, RunType::Once),
    cycleType(cycleType),
    cycleState(AnimationCycleState::Forward) {
    controller = &timer;
    timer.SetOnPlayTimeChangeFunc([this](auto& playable) {
        GUARD(onPlayTimeChangeFunc);
        onPlayTimeChangeFunc(*this);
    });
}

FinishType AnimationCycleTimer::OnUpdate(TimeSlice time) {
    GUARDR(!timer.IsFinished(), FinishType::Finish)

    timer.OnUpdate(time);

    GUARDR(timer.IsFinished(), FinishType::Continue)

    switch (cycleType) {
    case AnimationCycleType::Once:
        break;
    case AnimationCycleType::Loop:
        timer.Reset();

        if (onCycleFunc) {
            onCycleFunc(*this);
        }
        break;
    case AnimationCycleType::PingPong:
        cycleState = Flip(cycleState);
        timer.Reset();

        if (onCycleFunc) {
            onCycleFunc(*this);
        }
        break;
    }

    return IsFinished() ? FinishType::Finish : FinishType::Continue;
}

// MARK: - Playable

void AnimationCycleTimer::Reset() {
    Base::Reset();
    cycleState = AnimationCycleState::Forward;
}

float AnimationCycleTimer::PlayTime() const {
    switch (cycleState) {
    case AnimationCycleState::Reverse:
        return Duration() - timer.PlayTime();
        break;
    default:
        return timer.PlayTime();
    }
}

void AnimationCycleTimer::SetPlayTime(float value) {
    switch (cycleState) {
    case AnimationCycleState::Reverse:
        timer.SetPlayTime(Duration() - value);
        break;
    default:
        timer.SetPlayTime(value);
    }
}

float AnimationCycleTimer::Progress() const {
    switch (cycleState) {
    case AnimationCycleState::Reverse:
        return 1.0f - timer.Progress();
    default:
        return timer.Progress();
    }
}

void AnimationCycleTimer::SetProgress(float value) {
    switch (cycleState) {
    case AnimationCycleState::Reverse:
        timer.SetProgress(1.0f - value);
        break;
    default:
        timer.SetProgress(value);
        break;
    }
}
