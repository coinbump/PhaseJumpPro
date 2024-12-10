#pragma once

#include "Binding.h"
#include "ButtonControl.h"
#include "Macros.h"
#include "ObservedValue.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Verified
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// A button that when pressed, toggles off and on
    class ToggleButtonControl : public ButtonControl {
    public:
        using Base = ButtonControl;

        ToggleButtonControl(Binding<bool> isOnBinding = {});

        bool IsToggleOn() const;
        void SetIsToggleOn(bool value);
        void SetIsOnBinding(Binding<bool> value);

    protected:
        ObservedValue<bool> isOn;
        Binding<bool> isOnBinding;

        // MARK: ButtonControl

        void OnPress() override;
    };
} // namespace PJ
