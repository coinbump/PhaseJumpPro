#ifndef PJSDLMOUSEDEVICE_H
#define PJSDLMOUSEDEVICE_H

#include "SomeMouseDevice.h"

// CODE REVIEW: ?/23
namespace PJ {
    class SDLMouseDevice : public SomeMouseDevice {
    public:
        SDLMouseDevice() {}

        virtual ~SDLMouseDevice() {}

        ScreenPosition Position() const override;

        bool IsAvailable() override {
            return true;
        }

        bool IsButtonDown(PointerInputButtonType button) override;
    };
} // namespace PJ

#endif
