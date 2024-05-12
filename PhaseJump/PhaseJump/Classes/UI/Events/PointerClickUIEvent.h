#ifndef PJPOINTERCLICKUIEVENT_H
#define PJPOINTERCLICKUIEVENT_H

#include "SomePointerUIEvent.h"

namespace PJ {
    template <class Position>
    class PointerClickUIEvent : public SomePointerUIEvent {
    public:
        Position pressPosition;

        PointerClickUIEvent(Position pressPosition, InputButton button) : pressPosition(pressPosition) {
            this->button = button;
        }
    };

    template <class Position>
    class PointerDownUIEvent : public SomePointerUIEvent {
    public:
        Position pressPosition;

        PointerDownUIEvent(Position pressPosition, InputButton button) : pressPosition(pressPosition) {
            this->button = button;
        }
    };

    class PointerUpUIEvent : public SomeUIEvent {
    public:
        using InputButton = PointerInputButton;

        InputButton button;

        PointerUpUIEvent(InputButton button) : button(button) {
        }
    };

    template <class Position>
    class MouseMotionUIEvent : public SomeUIEvent {
    public:
        Position position;
        Vector2 delta;

        MouseMotionUIEvent(Position position, Vector2 delta) : position(position), delta(delta) {
        }
    };

    class PointerEnterUIEvent : public SomePointerUIEvent {};
    class PointerExitUIEvent : public SomePointerUIEvent {};

    /// Responds to pointer events
    class SomePointerEventsResponder {
    public:
        virtual ~SomePointerEventsResponder() {}
        
        virtual void OnPointerDownEvent(PointerDownUIEvent<LocalPosition> event) {}
        virtual void OnPointerEnterEvent(PointerEnterUIEvent event) {}
        virtual void OnPointerExitEvent(PointerExitUIEvent event) {}
        virtual void OnPointerUpEvent(PointerUpUIEvent event) {}
    };
}

#endif
