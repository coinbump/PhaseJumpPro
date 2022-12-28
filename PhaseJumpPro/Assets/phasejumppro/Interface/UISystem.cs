using System;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Manages common UI patterns like focus and drag and drop
    /// </summary>
    /// REQUIREMENTS:
    /// 1. Attach a Physics2DRaycaster to the camera
    /// 2. Create an EventSystem object and upgrade to the new Input system
    /// 3. Attach a non-trigger collider to objects that need interactive behavior
    public partial class UISystem : WorldComponent
    {
        public enum MouseOverAction
        {
            // Do nothing
            Ignore,

            // Give the object focus
            Focus
        }

        [NonSerialized]
        public static UISystem shared;

        /// <summary>
        /// Optional. Allows us to work with overlay cameras as well as the main camera
        /// </summary>
        public new Camera camera;

        /// <summary>
        /// Determine how a mouse over affects the state of the object under the cursor
        /// </summary>
        public MouseOverAction mouseOverAction;

        /// <summary>
        /// Item that has focus
        /// </summary>
        protected WeakReference<FocusHandler> focusedItem;
        protected WeakReference<SomeHoverGestureHandler> hoverItem;
        protected MouseDevice mouseDevice = new MouseDevice();
        protected ScreenPosition mousePosition;
        protected bool hasMousePosition = false;

        public Camera Camera
        {
            get
            {
                return camera != null ? camera : Camera.main;
            }
        }

        public FocusHandler ActiveFocus
        {
            get
            {
                if (null != focusedItem && focusedItem.TryGetTarget(out FocusHandler handler))
                {
                    return handler;
                }
                return null;
            }
            set
            {
                focusedItem = null == value ? null : new WeakReference<FocusHandler>(value);
            }
        }

        public SomeHoverGestureHandler ActiveHover
        {
            get
            {
                if (null != hoverItem && hoverItem.TryGetTarget(out SomeHoverGestureHandler handler))
                {
                    return handler;
                }
                return null;
            }
            set
            {
                if (null != hoverItem && hoverItem.TryGetTarget(out SomeHoverGestureHandler handler))
                {
                    if (handler == value) { return; }
                    handler.IsHovering = false;
                }

                hoverItem = null == value ? null : new WeakReference<SomeHoverGestureHandler>(value);

                if (value)
                {
                    value.IsHovering = true;
                }
            }
        }

        public UISystem()
        {
            // UISystem is a MonoBehavior and must be attached to an object in the scene
            // for Update events
            if (null == shared)
            {
                shared = this;
            }
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            var oldMousePosition = mousePosition;
            mousePosition = new ScreenPosition(mouseDevice.ScreenPosition);

            OnDragUpdate();

            if (!IsDragging)
            {
                // Update mouse over focus only when the mouse moves
                if (!hasMousePosition || !oldMousePosition.Equals(mousePosition))
                {
                    OnMouseMove();
                }
            }
            hasMousePosition = true;
        }
    }
}
