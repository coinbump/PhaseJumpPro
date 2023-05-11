#ifndef PJSDLMOUSEDEVICE_H
#define PJSDLMOUSEDEVICE_H

#include "SomeMouseDevice.h"

namespace PJ {
    class SDLMouseDevice : public SomeMouseDevice {
    public:
        SDLMouseDevice() {
        }

        ScreenPosition Position() const override;
        bool IsAvailable() override { return true; }
        bool IsButtonDown(PointerInputButton button) override;
    };
}

#endif
