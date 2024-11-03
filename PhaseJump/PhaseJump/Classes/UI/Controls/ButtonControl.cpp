#include "ButtonControl.h"
#include "Utils.h"

using namespace std;
using namespace PJ;

ButtonControl::ButtonControl() {
    signalFuncs[SignalId::PointerDown] = [](auto& component, auto& signal) {
        auto event = static_cast<PointerDownUIEvent const*>(&signal);
        static_cast<This*>(&component)->OnPointerDown(*event);
    };

    signalFuncs[SignalId::PointerEnter] = [](auto& component, auto& signal) {
        auto event = static_cast<PointerEnterUIEvent const*>(&signal);
        static_cast<This*>(&component)->OnPointerEnter(*event);
    };

    signalFuncs[SignalId::PointerExit] = [](auto& component, auto& signal) {
        auto event = static_cast<PointerExitUIEvent const*>(&signal);
        static_cast<This*>(&component)->OnPointerExit(*event);
    };

    signalFuncs[SignalId::PointerUp] = [](auto& component, auto& signal) {
        auto event = static_cast<PointerUpUIEvent const*>(&signal);
        static_cast<This*>(&component)->OnPointerUp(*event);
    };
}

void ButtonControl::Awake() {
    Base::Awake();
}

void ButtonControl::OnPointerDown(PointerDownUIEvent const& _event) {
    switch (trackType) {
    case TrackType::Immediate:
        OnPress();
        break;
    case TrackType::Track:
        isTracking = true;
        SetIsPressed(true);
        break;
    }
}

void ButtonControl::OnPointerEnter(PointerEnterUIEvent const& _event) {
    GUARD(isTracking)
    SetIsPressed(true);
}

void ButtonControl::OnPointerExit(PointerExitUIEvent const& _event) {
    GUARD(isTracking)
    SetIsPressed(false);
}

void ButtonControl::OnPointerUp(PointerUpUIEvent const& _event) {
    GUARD(isTracking)
    isTracking = false;

    bool wasPressed = isPressed;
    SetIsPressed(false);

    if (wasPressed) {
        OnPress();
    }
}

void ButtonControl::SetIsPressed(bool value) {
    GUARD(value != isPressed)
    isPressed = value;
    OnStateChange();
}

void ButtonControl::OnPress() {
    GUARD(onPressFunc)
    onPressFunc(*this);
}
