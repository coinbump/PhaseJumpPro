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
    public class Button : SomeHoverGestureHandler, IPointerDownHandler, IPointerUpHandler, IPointerEnterHandler, IPointerExitHandler, SomeGoStateListener<Button.StateType>
    {
        /// <summary>
        /// Button state
        /// IMPORTANT: Keep state IDs consistent for serialization
        /// </summary>
        public enum StateType
        {
            // Default
            Normal = 0,

            // Pointer (mouse) is hovering over
            Highlight = 1,

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
            get => core.State == StateType.Highlight;
        }

        protected bool isHovering = false;

        public override bool IsHovering
        {
            set
            {
                if (isTracking) { return; }
                isHovering = value;
                core.State = isHovering ? StateType.Highlight : StateType.Normal;
            }
        }

        public TrackingType trackingType = TrackingType.Track;

        protected GoCore<StateType> core;
        protected bool isTracking = false;

        public Button()
        {
            this.core = new(this);
        }

        public void OnPointerDown(PointerEventData eventData)
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

        public void OnPointerEnter(PointerEventData eventData)
        {
            if (!isTracking) { return; }
            core.State = StateType.Press;
        }

        public void OnPointerExit(PointerEventData eventData)
        {
            if (!isTracking) { return; }
            core.State = StateType.Normal;
        }

        public void OnPointerUp(PointerEventData eventData)
        {
            if (!isTracking) { return; }
            isTracking = false;

            bool wasPressed = false;
            switch (core.State)
            {
                case StateType.Press:
                    core.State = isHovering ? StateType.Highlight : StateType.Normal;
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
        }

        public void OnStateChange(GoStateMachine<StateType> inStateMachine)
        {
            var stateHandlers = GetComponentsInChildren<SomeStateHandler<Button.StateType>>();
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
