#ifndef PJSOMEPOINTERDEVICE_H
#define PJSOMEPOINTERDEVICE_H

#include "SomeInputDevice.h"
#include "SomePointerUIEvent.h"
#include "SomePosition.h"

// CODE REVIEW: ?/23
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

#endif
