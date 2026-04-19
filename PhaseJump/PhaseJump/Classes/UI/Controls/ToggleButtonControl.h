#pragma once

#include "Binding.h"
#include "ButtonTracker.h"
#include "Macros.h"
#include "ObservedValue.h"
#include "PointerClickUIEvent.h"
#include "StandardEventCore.h"
#include "View2D.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Verified
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// A button that when pressed, toggles off and on
    class ToggleButtonControl : public View2D {
    public:
        using This = ToggleButtonControl;
        using Base = View2D;

        using TrackType = ButtonTracker::TrackType;

        /// Encapsulates pointer tracking and press detection
        ButtonTracker tracker;

        ToggleButtonControl(Binding<bool> isOnBinding = {});

        bool IsToggleOn() const;
        void SetIsToggleOn(bool value);
        void SetIsOnBinding(Binding<bool> value);

        bool IsTracking() const {
            return tracker.IsTracking();
        }

        bool IsPressed() const {
            return tracker.IsPressed();
        }

    protected:
        ObservedValue<bool> isOn;
        Binding<bool> isOnBinding;

        /// Called when the button is pressed — flips the toggle state
        virtual void OnPress();

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "ToggleButtonControl";
        }
    };
} // namespace PJ
