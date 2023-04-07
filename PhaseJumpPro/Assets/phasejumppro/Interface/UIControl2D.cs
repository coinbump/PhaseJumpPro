using System;
using UnityEngine;
using UnityEngine.EventSystems;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 2/25/23
*/
namespace PJ
{
    /// <summary>
    /// An interactive UI object that accepts inputs from the user and produces events.
    /// Example: button, switch, checkbox, etc.
    /// </summary>
    public abstract class UIControl2D : View2D
    {
        [Header("UIControl Properties")]

        /// <summary>
        /// Component that handles events (Example: Button.PressEvent)
        /// </summary>
        public SomeEventHandler eventHandler;

        protected class HoverGestureHandler : SomeHoverGestureHandler
        {
            public WeakReference<UIControl2D> target;

            public override bool IsHovering
            {
                set
                {
                    if (target.TryGetTarget(out UIControl2D theTarget))
                    {
                        theTarget.IsHovering = value;
                    }
                }
            }
        }

        protected HoverGestureHandler hoverGestureHandler;

        protected bool isHovering = false;

        public virtual bool IsHovering
        {
            set
            {
                isHovering = value;
            }
        }

        public SomeListener Listener
        {
            get
            {
                SomeListener result = eventHandler;
                if (null == result)
                {
                    // Dispatch events to UISystem if no event handler is connected
                    result = UISystem.shared;
                }
                return result;
            }
        }

        protected override void Awake()
        {
            base.Awake();

            hoverGestureHandler = gameObject.AddComponent<HoverGestureHandler>();
            hoverGestureHandler.target = new(this);
        }
    }
}
