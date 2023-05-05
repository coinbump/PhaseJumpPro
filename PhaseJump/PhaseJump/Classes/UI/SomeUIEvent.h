#ifndef PJSOMEUIEVENT_H
#define PJSOMEUIEVENT_H

#include "FilePath.h"
#include "SomePosition.h"

namespace PJ {
    /// A user input event object
    class SomeUIEvent {
    public:
        virtual ~SomeUIEvent() {}
    };

    class DropFileUIEvent : public SomeUIEvent {
    public:
        FilePath filePath;

        DropFileUIEvent(FilePath filePath) : filePath(filePath) {
        }
    };

    class SomePointerUIEvent : public SomeUIEvent {
    public:
        enum class InputButton
        {
            Left,
            Right,
            Middle
        };

        InputButton button;
    };

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

    template <class Position>
    class PointerUpUIEvent : public SomePointerUIEvent {
        Position pressPosition;

        PointerUpUIEvent(Position pressPosition, InputButton button) : pressPosition(pressPosition) {
            this->button = button;
        }
    };

    class PointerEnterUIEvent : public SomePointerUIEvent {};
    class PointerExitUIEvent : public SomePointerUIEvent {};

    /// Responds to pointer events
    class SomePointerEventsHandler {
        virtual void OnPointerDownEvent(PointerDownUIEvent<LocalPosition> event) {}
        virtual void OnPointerEnterEvent(PointerEnterUIEvent event) {}
        virtual void OnPointerExitEvent(PointerExitUIEvent event) {}
        virtual void OnPointerUpEvent(PointerUpUIEvent<LocalPosition> event) {}
    };
}

#endif
