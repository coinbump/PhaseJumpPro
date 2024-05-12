#ifndef PJSONEPOINTERUIEVENT_H
#define PJSONEPOINTERUIEVENT_H

#include "SomeUIEvent.h"
#include "_String.h"

namespace PJ {
   enum class PointerInputButton
    {
        Left,
        Right,
        Middle,

        // A custom button
        Extended
    };

    // TODO: add InputButtonModel
    class SomePointerUIEvent : public SomeUIEvent {
    public:
        using InputButton = PointerInputButton;

        InputButton button;

        /// <summary>
        /// (OPTIONAL) id for non-standard button
        /// </summary>
        String buttonId;
    };
}

#endif
