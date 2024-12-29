#pragma once

#include "SomePointerUIEvent.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 8/9/24
 */
namespace PJ {
    /// Sent for a pointer down event followed by a pointer up event over the same visual element
    class PointerClickUIEvent : public SomePointerButtonUIEvent {
    public:
        using Base = SomePointerButtonUIEvent;
        using InputButton = PointerInputButtonType;

        PointerClickUIEvent(ScreenPosition pressPosition, InputButton button) :
            Base(button) {
            this->screenPos = pressPosition;
        }
    };

    /// Sent when a pointer button is pressed
    class PointerDownUIEvent : public SomePointerButtonUIEvent {
    public:
        using Base = SomePointerButtonUIEvent;
        using InputButton = PointerInputButtonType;

        PointerDownUIEvent(ScreenPosition pressPosition, InputButton button) :
            Base(button) {
            this->screenPos = pressPosition;
        }
    };

    /// Sent when a pointer button is released
    class PointerUpUIEvent : public SomePointerButtonUIEvent {
    public:
        using Base = SomePointerButtonUIEvent;
        using InputButton = PointerInputButtonType;

        PointerUpUIEvent(InputButton button) :
            Base(button) {}
    };

    /// Sent when the pointer moves
    class PointerMoveUIEvent : public SomeSignal {
    public:
        ScreenPosition screenPos;
        Vector2 delta;

        PointerMoveUIEvent(ScreenPosition position, Vector2 delta) :
            screenPos(position),
            delta(delta) {}
    };

    /// Sent when the pointer enters a visual element
    class PointerEnterUIEvent : public SomePointerUIEvent {
    public:
        using Base = SomePointerUIEvent;

        PointerEnterUIEvent() :
            Base() {}
    };

    /// Sent when the pointer exits a visual element
    class PointerExitUIEvent : public SomePointerUIEvent {
    public:
        using Base = SomePointerUIEvent;

        PointerExitUIEvent() :
            Base() {}
    };
} // namespace PJ
