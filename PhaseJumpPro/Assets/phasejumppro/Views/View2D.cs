using System;
using UnityEngine;
using System.Collections.Generic;

namespace PJ
{
    /// <summary>
    /// - Allows reading (right hand) coordinates where positive Y is down
    /// - Provides bounding frame for an object
    /// - Allows for standard UI system layouts and event management
    ///
    /// COORDINATE SYSTEMS:
    /// - World: World position in scene
    /// - Local: Local position in component's owner object
    /// - View: Position in view space, where the top left is (0, 0) inside the view frame and the bottom right is (frame.size.x, frame.size.y)
    /// 
    /// </summary>
    /// TBD: Do we need LateUpdate for ApplyLayout?
    public partial class View2D : NodeCore2D
    {
        private Bounds2D frame = new Bounds2D();

        [Header("View2D Properties")]
        /// <summary>
        /// Optional. Allows us to work with overlay cameras as well as the main camera
        /// </summary>
        public new Camera camera;

        protected Optional<float> intrinsicWidth;
        protected Optional<float> intrinsicHeight;

        public bool ignoreIntrinsicWidth = false;
        public bool ignoreIntrinsicHeight = false;

        /// <summary>
        /// Local z position for child view
        /// </summary>
        public float zStep = 0.1f;

        public virtual Optional<float> IntrinsicWidth
        {
            get
            {
                var fixedWidth = FixedWidth;
                if (null != fixedWidth)
                {
                    return fixedWidth;
                }

                if (ignoreIntrinsicWidth) { return null; }
                return intrinsicWidth;
            }
            set
            {
                intrinsicWidth = value;
            }
        }

        public float DefaultIntrinsicWidth
        {
            get
            {
                var result = IntrinsicWidth;
                if (null == result) { return 0; }
                return result.value;
            }
        }

        public virtual Optional<float> IntrinsicHeight
        {
            get
            {
                var fixedHeight = FixedHeight;
                if (null != fixedHeight)
                {
                    return fixedHeight;
                }

                if (ignoreIntrinsicHeight) { return null; }
                return intrinsicHeight;
            }
            set
            {
                intrinsicHeight = value;
            }
        }

        public float DefaultIntrinsicHeight
        {
            get
            {
                var result = IntrinsicHeight;
                if (null == result) { return 0; }
                return result.value;
            }
        }

        public Camera Camera
        {
            get
            {
                return camera != null ? camera : Camera.main;
            }
        }

        public View2D()
        {
        }

        public void UpdatePositions()
        {
            float baseZ = transform.localPosition.z;

            var zIndex = 0;
            foreach (Transform childTransform in gameObject.transform)
            {
                var childObject = childTransform.gameObject;
                var childView = childObject.GetComponent<View2D>();
                if (!childView) { continue; }

                var childPosition = childView.LocalPositionIn(Frame, transform.localPosition);

                var z = baseZ - (Mathf.Abs(zStep)) * (zIndex + 1);
                childPosition.z = z;

                childObject.transform.localPosition = childPosition;

                zIndex++;
            }

            foreach (Transform childTransform in gameObject.transform)
            {
                var childObject = childTransform.gameObject;
                var childView = childObject.GetComponent<View2D>();
                if (!childView) { continue; }

                childView.UpdatePositions();
            }
        }

        public View2D ParentView()
        {
            if (!transform.parent) { return null; }

            var parentGameObject = transform.parent.gameObject;
            if (null != parentGameObject)
            {
                if (parentGameObject.TryGetComponent(out View2D parentView))
                {
                    return parentView;
                }
            }
            return null;
        } // TESTED

        public List<View2D> ChildViews()
        {
            var result = new List<View2D>();

            foreach (Transform childTransform in transform)
            {
                if (childTransform.gameObject.TryGetComponent(out View2D view))
                {
                    result.Add(view);
                }
            }
            return result;
        } // TESTED

        public View2D FirstChildView()
        {
            foreach (Transform childTransform in transform)
            {
                if (childTransform.gameObject.TryGetComponent(out View2D view))
                {
                    return view;
                }
            }
            return null;
        }

        public List<View2D> FilteredChildViews(Func<View2D, bool> filter)
        {
            var result = new List<View2D>();

            foreach (Transform childTransform in transform)
            {
                if (childTransform.gameObject.TryGetComponent(out View2D view))
                {
                    if (filter(view))
                    {
                        result.Add(view);
                    }
                }
            }
            return result;
        }

        public View2D RootView()
        {
            var result = this;

            while (result.transform.parent)
            {
                var parentView = result.transform.parent.GetComponent<View2D>();
                if (!parentView) { break; }

                result = parentView;
            }

            return result;
        } // TESTED

        protected override void Start()
        {
            base.Start();

            ApplyLayout();
        }

        protected override void Awake()
        {
            base.Awake();

            UpdateFrameComponents();
        }
    }
}
