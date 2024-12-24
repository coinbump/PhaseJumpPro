#include "ButtonControl.h"
#include "Utils.h"

using namespace std;
using namespace PJ;

ButtonControl::ButtonControl() {
    AddSignalHandler<PointerDownUIEvent>({ .id = SignalId::PointerDown,
                                           .func = [this](auto& component, auto& event) {
                                               OnPointerDown(event);
                                           } });

    AddSignalHandler<PointerEnterUIEvent>({ .id = SignalId::PointerEnter,
                                            .func = [this](auto& component, auto& event) {
                                                OnPointerEnter(event);
                                            } });

    AddSignalHandler<PointerExitUIEvent>({ .id = SignalId::PointerExit,
                                           .func = [this](auto& component, auto& event) {
                                               OnPointerExit(event);
                                           } });

    AddSignalHandler<PointerUpUIEvent>({ .id = SignalId::PointerUp,
                                         .func = [this](auto& component, auto& event) {
                                             OnPointerUp(event);
                                         } });
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
    OnControlChange();
}

void ButtonControl::OnPress() {
    GUARD(onPressFunc)
    onPressFunc(*this);
}
