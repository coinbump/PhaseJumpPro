#pragma once

#include "SomeUIEvent.h"
#include "StringUtils.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/19/25
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

    /// Core for pointer button UI event
    class PointerButtonUIEventCore {
    public:
        using InputButton = PointerInputButtonType;

        ScreenPosition screenPos;
        InputButton button;
    };
} // namespace PJ
