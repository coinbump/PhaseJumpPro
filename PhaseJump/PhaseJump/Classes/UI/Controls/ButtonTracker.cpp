#include "ButtonTracker.h"
#include "Macros.h"

using namespace PJ;

void ButtonTracker::OnPointerDown() {
    switch (trackType) {
    case TrackType::Immediate:
        if (onPressFunc) {
            onPressFunc();
        }
        break;
    case TrackType::Track:
        isTracking = true;
        SetIsPressed(true);
        break;
    }
}

void ButtonTracker::OnPointerEnter() {
    GUARD(isTracking)
    SetIsPressed(true);
}

void ButtonTracker::OnPointerExit() {
    GUARD(isTracking)
    SetIsPressed(false);
}

void ButtonTracker::OnPointerUp() {
    GUARD(isTracking)
    isTracking = false;

    bool wasPressed = isPressed;
    SetIsPressed(false);

    if (wasPressed && onPressFunc) {
        onPressFunc();
    }
}

void ButtonTracker::SetIsPressed(bool value) {
    GUARD(value != isPressed)
    isPressed = value;
    if (onStateChangeFunc) {
        onStateChangeFunc();
    }
}
