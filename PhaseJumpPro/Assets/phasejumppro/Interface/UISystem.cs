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
    public partial class UISystem : MonoBehaviour
	{
		public static UISystem shared;

        /// <summary>
        /// Optional. Allows input system to work with overlay cameras
        /// </summary>
        public Camera inputCamera;

        /// <summary>
        /// Item that has focus
        /// </summary>
        protected WeakReference<Focusable> focusedItem;
        protected MouseInputController mouseInputController = new MouseInputController();

        public Camera InputCamera
        {
            get
            {
                return inputCamera != null ? inputCamera : Camera.main;
            }
        }

        Focusable ActiveFocus
        {
            get
            {
                if (null != focusedItem && focusedItem.TryGetTarget(out Focusable focusable))
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
                    focusedItem = new WeakReference<Focusable>(value);
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

            OnUpdateDrag();
        }
    }
}
