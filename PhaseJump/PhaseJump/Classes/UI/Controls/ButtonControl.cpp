#include "ButtonControl.h"
#include "SomeStateHandler.h"
#include "Utils.h"

using namespace std;
using namespace PJ;

ButtonControl::ButtonControl() {}

void ButtonControl::Awake() {
    Base::Awake();
}

void ButtonControl::OnPointerDown(PointerDownUIEvent _event) {
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

void ButtonControl::OnPointerEnter(PointerEnterUIEvent _event) {
    GUARD(isTracking)
    SetIsPressed(true);
}

void ButtonControl::OnPointerExit(PointerExitUIEvent _event) {
    GUARD(isTracking)
    SetIsPressed(false);
}

void ButtonControl::OnPointerUp(PointerUpUIEvent _event) {
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
