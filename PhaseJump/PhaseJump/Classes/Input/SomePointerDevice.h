#pragma once

#include "SomeInputDevice.h"
#include "SomePointerUIEvent.h"
#include "SomePosition.h"

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 9/22/24
 */
namespace PJ {
    /// Some device that sends pointer inputs (mouse, stylus)
    class SomePointerDevice : public SomeInputDevice {
    protected:
        ScreenPosition position;

    public:
        SomePointerDevice() {}

        virtual ScreenPosition Position() const {
            return position;
        }

        virtual bool IsButtonDown(PointerInputButtonType button) {
            return false;
        }
    };
} // namespace PJ
