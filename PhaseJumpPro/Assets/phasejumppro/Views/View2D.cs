using System;
using UnityEngine;
using System.Collections.Generic;
using System.Linq;

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
        private Bounds2D frame = new();

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
        public float zStep = 0.001f;

        private bool needsLayout = true;

        public bool NeedsLayout
        {
            get => needsLayout;
            set
            {
                this.needsLayout = value;
                if (needsLayout)
                {
                    var parentView = ParentView();
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

                if (ignoreIntrinsicWidth) { return null; }

                if (TryGetComponent(out SpriteRenderer spriteRenderer))
                {
                    var scale = spriteRenderer.gameObject.transform.localScale;

                    switch (spriteRenderer.drawMode)
                    {
                        case SpriteDrawMode.Sliced:
                        case SpriteDrawMode.Tiled:
                            break;
                        case SpriteDrawMode.Simple:
                            var bounds = spriteRenderer.sprite.bounds;
                            return new(bounds.size.x * scale.x);
                    }
                }

                return intrinsicWidth;
            }
            set
            {
                intrinsicWidth = value;
            }
        }

        protected void UpdateSpriteRenderer()
        {
            if (TryGetComponent(out SpriteRenderer spriteRenderer))
            {
                var spriteSize = SpriteRendererSize(spriteRenderer);
                if (null != spriteSize)
                {
                    spriteRenderer.size = spriteSize.value;
                }
            }
        }

        protected virtual Optional<Vector2> SpriteRendererSize(SpriteRenderer spriteRenderer)
        {
            switch (spriteRenderer.drawMode)
            {
                case SpriteDrawMode.Sliced:
                case SpriteDrawMode.Tiled:
                    return new(Frame.size);
                case SpriteDrawMode.Simple:
                    break;
            }

            return null;
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

                if (TryGetComponent(out SpriteRenderer spriteRenderer))
                {
                    var result = IntrinsicHeightFor(spriteRenderer);
                    if (null != result)
                    {
                        return result;
                    }
                }

                return intrinsicHeight;
            }
            set
            {
                intrinsicHeight = value;
            }
        }

        public Optional<float> IntrinsicHeightFor(SpriteRenderer spriteRenderer)
        {
            var scale = spriteRenderer.gameObject.transform.localScale;

            switch (spriteRenderer.drawMode)
            {
                case SpriteDrawMode.Sliced:
                case SpriteDrawMode.Tiled:
                    break;
                case SpriteDrawMode.Simple:
                    var bounds = spriteRenderer.sprite.bounds;
                    return new(bounds.size.y * scale.y);
            }

            return null;
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

                var z = baseZ + (Mathf.Abs(zStep) * Vector3.forward.z) * (zIndex + 1);
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
        }

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
