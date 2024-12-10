#pragma once

#include "PointerClickUIEvent.h"
#include "StandardEventCore.h"
#include "UIControl2D.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 11/29/24
 */
namespace PJ {
    /// Handles button tracking behavior for a button control
    class ButtonControl : public UIControl2D {
    public:
        using This = ButtonControl;
        using Base = UIControl2D;

        using OnPressFunc = std::function<void(ButtonControl&)>;

    protected:
        bool isTracking = false;
        bool isPressed = false;

        void SetIsPressed(bool value);

    public:
        enum class TrackType {
            // Track the button
            Track,

            // Fire the button press on tap
            Immediate
        };

        TrackType trackType = TrackType::Track;

        /// Called when the button is pressed
        OnPressFunc onPressFunc;

        bool IsTracking() const {
            return isTracking;
        }

        bool IsPressed() const {
            return isPressed;
        }

    public:
        ButtonControl();

        void OnPointerDown(PointerDownUIEvent const& _event);
        void OnPointerEnter(PointerEnterUIEvent const& _event);
        void OnPointerExit(PointerExitUIEvent const& _event);
        void OnPointerUp(PointerUpUIEvent const& _event);

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
