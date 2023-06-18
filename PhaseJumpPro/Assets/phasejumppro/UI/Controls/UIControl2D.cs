using System;
using UnityEngine;
using UnityEngine.EventSystems;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 6/20/23
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// An interactive UI object that accepts inputs from the user and produces events.
    /// Example: button, switch, checkbox, etc.
    /// </summary>
    public abstract class UIControl2D : View2D
    {
        [Header("UIControl2D Properties")]

        /// <summary>
        /// Component that handles events (Example: Button.PressEvent)
        /// </summary>
        public SomeEventHandler eventHandler;

        protected void Reset()
        {
            isFixedWidth = true;
            isFixedHeight = true;
        }

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
                    // Allows us to centralize UI logic if needed
                    result = this.UISystem;
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
