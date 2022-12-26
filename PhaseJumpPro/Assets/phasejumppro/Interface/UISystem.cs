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
        public static UISystem shared;

        /// <summary>
        /// Optional. Allows us to work with overlay cameras as well as the main camera
        /// </summary>
        public new Camera camera;

        /// <summary>
        /// Item that has focus
        /// </summary>
        protected WeakReference<FocusHandler> focusedItem;
        protected MouseDevice mouseDevice = new MouseDevice();

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
                if (null != focusedItem && focusedItem.TryGetTarget(out FocusHandler focusable))
                {
                    return focusable;
                }
                return null;
            }
            set
            {
                if (null == value)
                {
                    focusedItem = null;
                }
                else
                {
                    focusedItem = new WeakReference<FocusHandler>(value);
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

            OnDragUpdate();
        }
    }
}
