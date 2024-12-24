#include "Playable.h"

using namespace std;
using namespace PJ;

Playable::Playable(PlayState defaultState) :
    playState(defaultState),
    defaultState(defaultState) {}

void Playable::SetPlayState(PlayState value) {
    GUARD(value != playState)
    playState = value;
    OnPlayStateChange();
}

void Playable::OnPlayStateChange() {
    GUARD(onPlayStateChangeFunc)
    onPlayStateChangeFunc(*this);
}

void Playable::SetOnPlayStateChangeFunc(OnPlayStateChangeFunc value) {
    onPlayStateChangeFunc = value;

    // Synchronize state
    OnPlayStateChange();
}

void Playable::SetOnPlayTimeChangeFunc(OnPlayTimeChangeFunc value) {
    onPlayTimeChangeFunc = value;

    // Synchronize state
    OnPlayTimeChange();
}

bool Playable::HasController() const {
    return Controller() != nullptr;
}

void Playable::Dispatch(std::function<void(Playable&)> func) {
    auto controller = Controller();
    GUARD(controller)
    func(*controller);
}

void Playable::Reset() {
    if (HasController()) {
        Dispatch([](auto& controller) { controller.Reset(); });
    } else {
        SetPlayTime(0);
        SetIsFinished(false);
        SetPlayState(defaultState);
    }
}

float Playable::Duration() const {
    return DispatchResult<float>([](Playable& controller) { return controller.Duration(); });
}

void Playable::SetDuration(float value) {
    Dispatch([=](auto& controller) { controller.SetDuration(value); });
}

float Playable::Progress() const {
    return DispatchResult<float>([](Playable& controller) { return controller.Progress(); });
}

void Playable::SetProgress(float value) {
    Dispatch([=](auto& controller) { controller.SetProgress(value); });
}

void Playable::Play() {
    if (HasController()) {
        Dispatch([](auto& controller) { controller.Play(); });
    } else {
        SetPlayState(PlayState::Play);
    }
}

void Playable::Pause() {
    if (HasController()) {
        Dispatch([](auto& controller) { controller.Pause(); });
    } else {
        SetPlayState(PlayState::Pause);
    }
}

void Playable::Stop() {
    if (HasController()) {
        Dispatch([](auto& controller) { controller.Stop(); });
    } else {
        SetPlayState(PlayState::Stop);
    }
}

bool Playable::IsPaused() const {
    if (HasController()) {
        return DispatchResult<bool>([](Playable& controller) { return controller.IsPaused(); });
    } else {
        return playState == PlayState::Pause;
    }
}

bool Playable::IsPlaying() const {
    GUARDR(!IsFinished(), false)
    if (HasController()) {
        return DispatchResult<bool>([](Playable& controller) { return controller.IsPlaying(); });
    } else {
        return playState == PlayState::Play;
    }
}

float Playable::PlayTime() const {
    return DispatchResult<float>([](Playable& controller) { return controller.PlayTime(); });
}

void Playable::SetPlayTime(float time) {
    GUARD(PlayTime() != time);
    Dispatch([=](auto& controller) { controller.SetPlayTime(time); });
    OnPlayTimeChange();
}

void Playable::OnPlayTimeChange() {
    GUARD(onPlayTimeChangeFunc);
    onPlayTimeChangeFunc(*this);
}

void Playable::OnUpdate(TimeSlice time) {
    if (HasController()) {
        Dispatch([=](auto& controller) { controller.OnUpdate(time); });
    } else {
        Base::OnUpdate(time);
    }
}

bool Playable::IsFinished() const {
    GUARDR(!Base::IsFinished(), true);
    return DispatchResult<bool>([](Playable& controller) { return controller.IsFinished(); });
}
