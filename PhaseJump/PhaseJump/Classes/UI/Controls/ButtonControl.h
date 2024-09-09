#pragma once

#include "ComposeTimedStateMachine.h"
#include "StandardEventCore.h"
#include "UIControl2D.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/4/24
 */
namespace PJ {
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

        // MARK: SomePointerUIEventsResponder

        void OnPointerDown(PointerDownUIEvent _event) override;
        void OnPointerEnter(PointerEnterUIEvent _event) override;
        void OnPointerExit(PointerExitUIEvent _event) override;
        void OnPointerUp(PointerUpUIEvent _event) override;

    protected:
        void Awake() override;

        /// Called when the button is pressed
        virtual void OnPress();
    };
} // namespace PJ
