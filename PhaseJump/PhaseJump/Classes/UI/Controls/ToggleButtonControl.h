#pragma once

#include "ButtonControl.h"
#include "Macros.h"
#include "SomeEffect.h"

/*
 RATING: 5 stars
 Verified
 CODE REVIEW: 9/4/24
 */
namespace PJ {
    /// A button that, when pressed, toggles off and on
    class ToggleButtonControl : public ButtonControl {
    public:
        using Base = ButtonControl;

        bool IsToggleOn() const {
            return isToogleOn;
        }

        void SetIsToggleOn(bool value) {
            GUARD(isToogleOn != value)
            isToogleOn = value;
            OnStateChange();
        }

    protected:
        bool isToogleOn = false;

        // MARK: ButtonControl

        void OnPress() override {
            Base::OnPress();

            SetIsToggleOn(!IsToggleOn());
        }
    };
} // namespace PJ
