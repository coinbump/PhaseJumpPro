using System;
using UnityEngine;
using System.Collections.Generic;
using System.Linq;

/*
RATING: 4 stars
Tested and works
Needs unit tests
CODE REVIEW: 6/10/23
*/
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
    /// SPECIAL BEHAVIOR:
    /// Sprite Renderers get special logic because they are a core of game programming, and many views will have a SpriteRenderer component
    /// This allows us to use all views for sprites without adding extra complexity
    /// (NOTE: When porting to other languages, we can modify the renderer code and have it return intrinsic sizes, but not here)
    /// </summary>
    /// TBD: Do we need LateUpdate for ApplyLayout?
    public partial class View2D : Node2D
    {
        private Rect frame = new();

        [Header("View2D Properties")]
        /// <summary>
        /// Optional. Allows us to work with overlay cameras as well as the main camera
        /// </summary>
        public new Camera camera;

        protected Optional<float> intrinsicWidth;
        protected Optional<float> intrinsicHeight;

        /// <summary>
        /// If true, the width is calculated based on the view's intrinsic size
        /// </summary>
        public bool isFixedWidth;

        /// <summary>
        /// If true, the height is calculated based on the view's intrisic size
        /// </summary>
        public bool isFixedHeight;

        /// <summary>
        /// z step to child view
        /// </summary>
        public float zStep = 0.001f;

        /// <summary>
        /// If true, layout is invalid and needs to be recalculated
        /// </summary>
        private bool needsLayout = true;

        public SomeViewModifier2D viewModifier;
        public SomeDependencyResolver<UISystem> uiSystemDependencyResolver = new UISystemSharedDependencyResolver();

        public UISystem UISystem => uiSystemDependencyResolver.Dependency();

        public bool NeedsLayout
        {
            get => needsLayout;
            set
            {
                this.needsLayout = value;
                if (needsLayout)
                {
                    // Child layout affects parent layout
                    var parentView = ParentView;
                    if (parentView)
                    {
                        parentView.NeedsLayout = true;
                    }
                }
            }
        }

        public virtual Optional<float> IntrinsicWidth
        {
            get
            {
                var fixedWidth = FixedWidth;
                if (null != fixedWidth)
                {
                    return fixedWidth;
                }

                if (null != viewModifier)
                {
                    var modifierIntrinsicWidth = viewModifier.IntrinsicWidth(this);
                    if (null != modifierIntrinsicWidth)
                    {
                        return modifierIntrinsicWidth;
                    }
                }

                return intrinsicWidth;
            }
            set
            {
                intrinsicWidth = value;
            }
        }

        protected void OnViewSizeChange()
        {
            if (null == viewModifier) { return; }
            viewModifier.OnViewSizeChange(this);
        }

        /// <summary>
        /// The intrinsic width if it exists, or zero
        /// </summary>
        public float IntrinsicWidthOrNone
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

                if (null != viewModifier)
                {
                    var modifierIntrinsicHeight = viewModifier.IntrinsicHeight(this);
                    if (null != modifierIntrinsicHeight)
                    {
                        return modifierIntrinsicHeight;
                    }
                }

                return intrinsicHeight;
            }
            set
            {
                intrinsicHeight = value;
            }
        }

        /// <summary>
        /// Returns the intrinsic height, or zero
        /// </summary>
        public float IntrinsicHeightOrNone
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
            get => camera != null ? camera : Camera.main;
        }

        public View2D()
        {
        }

        public void UpdatePositions()
        {
            float baseZ = 0;

            var zIndex = 0;
            foreach (Transform childTransform in gameObject.transform)
            {
                var childObject = childTransform.gameObject;
                var childView = childObject.GetComponent<View2D>();
                if (!childView) { continue; }

                var childPosition = childView.LocalPositionIn(Frame, transform.localPosition);

                var z = baseZ + (Mathf.Abs(zStep) * Vector3.back.z) * (zIndex + 1);
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

        public View2D ParentView
        {
            get
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
            }
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

        /// <summary>
        /// Collect depth-first graph of views
        /// </summary>
        public List<View2D> GraphViews()
        {
            var result = new List<View2D>();
            result.Add(this);

            foreach (Transform childTransform in transform)
            {
                if (childTransform.gameObject.TryGetComponent(out View2D view))
                {
                    result.AddRange(view.GraphViews());
                }
            }
            return result;
        } // TESTED

        public View2D FirstChildView
        {
            get
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
        } // TESTED

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

        public View2D RootView
        {
            get
            {
                var result = this;

                while (result.transform.parent)
                {
                    var parentView = result.transform.parent.GetComponent<View2D>();
                    if (!parentView) { break; }

                    result = parentView;
                }

                return result;
            }
        } // TESTED

        protected override void Start()
        {
            base.Start();

            ApplyLayout(true);
        }

        protected override void Awake()
        {
            base.Awake();

            UpdateFrameComponents();
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            ApplyLayout(false);
        }
    }
}
