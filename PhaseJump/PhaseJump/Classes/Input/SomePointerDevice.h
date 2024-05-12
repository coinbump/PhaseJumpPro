#ifndef PJSOMEPOINTERDEVICE_H
#define PJSOMEPOINTERDEVICE_H

#include "SomeInputDevice.h"
#include "SomePosition.h"
#include "SomePointerUIEvent.h"

namespace PJ
{
    /// <summary>
    /// Some device that sends pointer inputs (mouse, stylus)
    /// </summary>
    class SomePointerDevice : public SomeInputDevice
    {
    protected:
        ScreenPosition position;

    public:
        SomePointerDevice() {
        }

        virtual ScreenPosition Position() const { return position; }
        virtual bool IsButtonDown(PointerInputButton button) { return false; }
    };
}

#endif
