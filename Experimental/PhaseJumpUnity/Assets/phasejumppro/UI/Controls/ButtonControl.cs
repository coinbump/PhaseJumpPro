using UnityEngine;
using UnityEngine.EventSystems;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 6/10/23
PORTED TO: C++
*/
namespace PJ
{
    // FUTURE: ButtonView wraps this
    public class ButtonControl : UIControl2D, SomeGoStateListener<ButtonControl.StateType>
    {
        public class PressEvent : Event
        {
            public PressEvent(string id) : base(id)
            {
            }
        }

        /// <summary>
        /// Button state
        /// IMPORTANT: Keep state IDs consistent for serialization
        /// </summary>
        public enum StateType
        {
            // Default
            Normal = 0,

            // Pointer (mouse) is hovering over
            Hover = 1,

            // Is pressed while tracking pointer input
            Press = 2,

            // Disabled
            Disabled = 3
        }

        public enum TrackingType
        {
            // Track the button
            Track,

            // Fire the button press on tap
            Immediate
        }

        public override bool IsHovering
        {
            set
            {
                isHovering = value;
                if (isTracking) { return; }

                core.State = isHovering ? StateType.Hover : StateType.Normal;
            }
        }

        [Header("Button Properties")]

        public TrackingType trackingType = TrackingType.Track;

        protected GoCore<StateType> core;
        protected bool isTracking = false;

        public ButtonControl()
        {
            this.core = new(this);
        }

        public override void OnPointerDownEvent(PointerDownUIEvent<LocalPosition> _event)
        {
            switch (trackingType)
            {
                case TrackingType.Immediate:
                    OnPress();
                    break;
                case TrackingType.Track:
                    isTracking = true;
                    core.State = StateType.Press;
                    break;
            }
        }

        public override void OnPointerEnterEvent(PointerEnterUIEvent _event)
        {
            if (!isTracking) { return; }
            core.State = StateType.Press;
        }

        public override void OnPointerExitEvent(PointerExitUIEvent _event)
        {
            if (!isTracking) { return; }
            core.State = StateType.Normal;
        }

        public override void OnPointerUpEvent(PointerUpUIEvent _event)
        {
            if (!isTracking) { return; }
            isTracking = false;

            bool wasPressed = false;
            switch (core.State)
            {
                case StateType.Press:
                    core.State = isHovering ? StateType.Hover : StateType.Normal;
                    wasPressed = true;
                    break;
                default:
                    core.State = StateType.Normal;
                    break;
            }

            if (wasPressed)
            {
                OnPress();
            }
        }

        /// <summary>
        /// Called when the button is pressed
        /// </summary>
        protected virtual void OnPress()
        {
            // Debug.Log("Button Pressed");
            var listener = Listener;
            if (null == listener) { return; }
            listener.OnEvent(new PressEvent(id));
        }

        public void OnStateChange(GoStateMachine<StateType> inStateMachine)
        {
            // Forward button state change to state handler components
            var stateHandlers = GetComponentsInChildren<SomeStateHandler<ButtonControl.StateType>>();
            foreach (var stateHandler in stateHandlers)
            {
                stateHandler.OnStateChange(inStateMachine.State);
            }
        }

        public void OnStateFinish(GoStateMachine<StateType> inStateMachine)
        {
        }
    }
}
