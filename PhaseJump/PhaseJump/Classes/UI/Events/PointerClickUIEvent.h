#pragma once

#include "PointerUIEventTypes.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 8/9/24
 */
namespace PJ {
    /// Sent for a pointer down event followed by a pointer up event over the same visual element
    class PointerClickUIEvent : public SomeSignal {
    public:
        using Base = SomeSignal;
        using InputButton = PointerInputButtonType;
        using Core = PointerButtonUIEventCore;

        Core core;

        PointerClickUIEvent(Core const& core) :
            core(core) {}
    };

    /// Sent when a pointer button is pressed
    class PointerDownUIEvent : public SomeSignal {
    public:
        using Base = SomeSignal;
        using InputButton = PointerInputButtonType;
        using Core = PointerButtonUIEventCore;

        Core core;

        PointerDownUIEvent(Core const& core) :
            core(core) {}
    };

    /// Sent when a pointer button is released
    class PointerUpUIEvent : public SomeSignal {
    public:
        using Base = SomeSignal;
        using InputButton = PointerInputButtonType;
        using Core = PointerButtonUIEventCore;

        Core core;

        PointerUpUIEvent(Core const& core) :
            core(core) {}
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
    class PointerEnterUIEvent : public SomeSignal {
    public:
        using Base = SomeSignal;

        ScreenPosition screenPos;

        PointerEnterUIEvent() :
            Base() {}
    };

    /// Sent when the pointer exits a visual element
    class PointerExitUIEvent : public SomeSignal {
    public:
        using Base = SomeSignal;

        ScreenPosition screenPos;

        PointerExitUIEvent() :
            Base() {}
    };
} // namespace PJ
