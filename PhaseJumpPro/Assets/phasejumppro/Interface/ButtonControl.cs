using UnityEngine;
using UnityEngine.EventSystems;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/27/22
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

        public bool IsHighlighted
        {
            get => core.State == StateType.Hover;
        }

        public override bool IsHovering
        {
            set
            {
                if (isTracking) { return; }
                isHovering = value;
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

        public override void OnPointerDown(PointerEventData eventData)
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

        public override void OnPointerEnter(PointerEventData eventData)
        {
            if (!isTracking) { return; }
            core.State = StateType.Press;
        }

        public override void OnPointerExit(PointerEventData eventData)
        {
            if (!isTracking) { return; }
            core.State = StateType.Normal;
        }

        public override void OnPointerUp(PointerEventData eventData)
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
