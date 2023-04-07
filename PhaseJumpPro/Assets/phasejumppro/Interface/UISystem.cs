using System;
using System.Collections.Generic;
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
    public partial class UISystem : WorldComponent, SomeListener
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

        [Header("Layer Properties")]

        /// <summary>
        /// Z position of the backmost layer
        /// </summary>
        public float layerZStart;

        /// <summary>
        /// Step between layers
        /// </summary>
        public float layerZStep = 1.0f;

        public List<UILayer> layers = new();

        /// <summary>
        /// Item that has focus
        /// </summary>
        protected WeakReference<FocusHandler> focusedItem;
        protected WeakReference<GameObject> hoverItem;
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

        public GameObject ActiveHover
        {
            get
            {
                if (null != hoverItem && hoverItem.TryGetTarget(out GameObject target))
                {
                    return target;
                }
                return null;
            }
            set
            {
                var activeHover = ActiveHover;
                if (activeHover == value) { return; }

                if (activeHover)
                {
                    var hoverHandlers = activeHover.GetComponents<SomeHoverGestureHandler>();
                    foreach (var handler in hoverHandlers)
                    {
                        handler.IsHovering = false;
                    }
                }

                hoverItem = null;

                if (value)
                {
                    hoverItem = new WeakReference<GameObject>(value);

                    var hoverHandlers = value.GetComponents<SomeHoverGestureHandler>();
                    foreach (var handler in hoverHandlers)
                    {
                        handler.IsHovering = true;
                    }
                }
            }
        }

        protected override void Awake()
        {
            base.Awake();

            // Only the first UISystem is shared (avoids problems when loading in additive scenes)
            if (null == shared)
            {
                shared = this;
            }
        }

        protected override void Start()
        {
            base.Start();

            UpdateLayers();
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

        public virtual void OnEvent(Event _event)
        {
            // Respond to events by overriding in subclass
        }
    }
}
