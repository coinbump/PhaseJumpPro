#pragma once

#include "SomeMouseDevice.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/21/24
 */
namespace PJ {
    class SDLMouseDevice : public SomeMouseDevice {
    public:
        SDLMouseDevice() {}

        virtual ~SDLMouseDevice() {}

        // MARK: SomeMouseDevice

        ScreenPosition GetScreenPosition() const override;

        bool IsAvailable() override {
            return true;
        }

        bool IsButtonDown(PointerInputButtonType button) override;
    };
} // namespace PJ
