#pragma once

#include "PointerUIEventTypes.h"
#include "SomeInputDevice.h"
#include "SomePosition.h"

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 9/22/24
 */
namespace PJ {
    /// Some device that sends pointer inputs (mouse, stylus)
    class SomePointerDevice : public SomeInputDevice {
    public:
        SomePointerDevice() {}

        virtual ScreenPosition GetScreenPosition() const = 0;

        virtual bool IsButtonDown(PointerInputButtonType button) = 0;
    };
} // namespace PJ
