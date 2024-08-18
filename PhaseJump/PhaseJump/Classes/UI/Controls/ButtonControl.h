#ifndef PJBUTTONCONTROL_H
#define PJBUTTONCONTROL_H

#include "ComposeTimedStateMachine.h"
#include "StandardEventCore.h"
#include "UIControl2D.h"

/*
 RATING: 4 stars
 Tested and works. Needs unit tests
 CODE REVIEW: 5/8/23
 */
namespace PJ {
    /// Button state
    // TODO: Use struct-composition for these, not enum
    enum class ButtonControlStateType {
        // Default
        Normal,

        // Pointer (mouse) is hovering over
        Hover,

        // Is pressed while tracking pointer input
        Press,

        // Disabled
        Disabled
    };

    // FUTURE: ButtonView wraps this
    class ButtonControl : public UIControl2D,
                          public SomeTimedStateMachineOwner<ButtonControlStateType> {
    public:
        using Base = UIControl2D;
        using StateType = ButtonControlStateType;

        class PressEvent : public Event<StandardEventCore> {
        public:
            using Base = Event<StandardEventCore>;

            PressEvent(String id) :
                Base(StandardEventCore(id)) {}
        };

        // TODO: Use struct-composition for these, not enum
        enum class TrackingType {
            // Track the button
            Track,

            // Fire the button press on tap
            Immediate
        };

        void SetIsHovering(bool value) override;

        TrackingType trackingType = TrackingType::Track;

    protected:
        UP<ComposeTimedStateMachine<StateType>> composeTimedStateMachine;
        bool isTracking = false;

    public:
        ButtonControl();

        void OnPointerDown(PointerDownUIEvent _event) override;
        void OnPointerEnter(PointerEnterUIEvent _event) override;
        void OnPointerExit(PointerExitUIEvent _event) override;
        void OnPointerUp(PointerUpUIEvent _event) override;

    protected:
        void Awake() override;

        /// Called when the button is pressed
        virtual void OnPress();

        void OnStateChange(TimedStateMachine<StateType>& inStateMachine) override;
        void OnStateFinish(TimedStateMachine<StateType>& inStateMachine) override;
    };
} // namespace PJ

#endif
