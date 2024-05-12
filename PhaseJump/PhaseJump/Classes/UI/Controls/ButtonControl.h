#ifndef PJBUTTONCONTROL_H
#define PJBUTTONCONTROL_H

#include "UIControl2D.h"
#include "Event.h"
#include "GoCore.h"

/*
 RATING: 4 stars
 Tested and works. Needs unit testse
 CODE REVIEW: 5/8/23
 */
namespace PJ
{
    /// <summary>
    /// Button state
    /// </summary>
    enum class ButtonControlStateType
    {
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
    class ButtonControl : public UIControl2D, public SomeGoStateListener<ButtonControlStateType>
    {
    public:
        using Base = UIControl2D;
        using StateType = ButtonControlStateType;

        class PressEvent : public Event
        {
        public:
            using Base = Event;

            PressEvent(String id) : Base(id)
            {
            }
        };

        enum class TrackingType
        {
            // Track the button
            Track,

            // Fire the button press on tap
            Immediate
        };

        void SetIsHovering(bool value) override;
        
        TrackingType trackingType = TrackingType::Track;

    protected:
        SP<GoCore<StateType>> core = MAKE<GoCore<StateType>>();
        bool isTracking = false;

    public:
        ButtonControl();

        void OnPointerDownEvent(PointerDownUIEvent<LocalPosition> _event) override;
        void OnPointerEnterEvent(PointerEnterUIEvent _event) override;
        void OnPointerExitEvent(PointerExitUIEvent _event) override;
        void OnPointerUpEvent(PointerUpUIEvent _event) override;

    protected:
        void Awake() override;

        /// Called when the button is pressed
        virtual void OnPress();

        void OnStateChange(GoStateMachine<StateType>& inStateMachine) override;
        void OnStateFinish(GoStateMachine<StateType>& inStateMachine) override;
    };
}

#endif
