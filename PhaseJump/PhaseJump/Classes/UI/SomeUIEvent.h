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
        enum class InputButton
        {
            Left,
            Right,
            Middle
        };
    };

    class PointerClickUIEvent : public SomePointerUIEvent {
    public:
        LocalPosition pressPosition;
    };

    class PointerDownUIEvent : public SomePointerUIEvent {
        LocalPosition pressPosition;
    };

    class PointerUpUIEvent : public SomePointerUIEvent {
    };
}

#endif
