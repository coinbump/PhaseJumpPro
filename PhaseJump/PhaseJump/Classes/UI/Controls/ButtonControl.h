#pragma once

#include "ButtonTracker.h"
#include "PointerClickUIEvent.h"
#include "StandardEventCore.h"
#include "View2D.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 11/29/24
 */
namespace PJ {
    /// Handles button tracking behavior for a button control
    class ButtonControl : public View2D {
    public:
        using This = ButtonControl;
        using Base = View2D;

        using OnPressFunc = std::function<void(ButtonControl&)>;
        using TrackType = ButtonTracker::TrackType;

        /// Encapsulates pointer tracking and press detection
        ButtonTracker tracker;

        /// Called when the button is pressed
        OnPressFunc onPressFunc;

        bool IsTracking() const {
            return tracker.IsTracking();
        }

        bool IsPressed() const {
            return tracker.IsPressed();
        }

        ButtonControl();

    protected:
        void Awake() override;

        /// Called when the button is pressed
        virtual void OnPress();

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "ButtonControl";
        }
    };
} // namespace PJ
