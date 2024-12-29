#pragma once

#include "SomeUIEvent.h"
#include "StringUtils.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/4/24
 */
namespace PJ {
    using PointerInputButtonType = String;

    /// Standard pointer input button ids
    /// Create your own ids for custom behavior
    namespace PointerInputButton {
        auto constexpr Left = "left";
        auto constexpr Right = "right";
        auto constexpr Middle = "middle";
    } // namespace PointerInputButton

    /// Pointer device UI event
    class SomePointerUIEvent : public SomeSignal {
    public:
        ScreenPosition screenPos;

        SomePointerUIEvent() {}
    };

    /// Pointer button UI event
    class SomePointerButtonUIEvent : public SomePointerUIEvent {
    public:
        using InputButton = PointerInputButtonType;

        InputButton button;

        SomePointerButtonUIEvent(InputButton button) :
            button(button) {}
    };
} // namespace PJ
