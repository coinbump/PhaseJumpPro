using System;
using UnityEngine;
using UnityEditor;

namespace PJ
{
    /// <summary>
    /// Allows gameObjects to be treated as 2D views in reading (right hand) coordinates where positive Y is down
    /// Each view has a frame and applies the layout for its child views
    /// </summary>
    /// TBD: Do we need LateUpdate for ApplyLayout?
    public class View2D : Node
    {
        /// <summary>
        /// Frame in view space (positive Y is down)
        /// </summary>
        public Bounds2D frame;

        /// <summary>
        /// Local bounds (origin is always zero)
        /// </summary>
        public Bounds2D Bounds
        {
            get
            {
                var result = frame;
                result.origin = Vector2.zero;
                return result;
            }
        }

        /// <summary>
        /// Returns the top-left world position of this view (topmost view only)
        /// Do not use this for child views
        /// </summary>
        public Vector3 TopLeftWorldPosition
        {
            get
            {
                var origin = transform.position;
                var topLeft = new Vector3(
                    origin.x - WorldSize.x / 2.0f,
                    origin.y + WorldSize.y / 2.0f * Vector2.up.y,
                    transform.position.z
                );
                return topLeft;
            }
        }

        /// <summary>
        /// Returns the top-left local position of this view
        /// </summary>
        public Vector3 TopLeftLocalPositionIn(Bounds2D parentFrame)
        {
            var topLeft = new Vector3(
                -parentFrame.size.x / 2.0f + frame.origin.x,
                parentFrame.size.y / 2.0f * Vector2.up.y + frame.origin.y * Vector2.down.y,
                transform.localPosition.z
            );
            return topLeft;
        }

        public Vector3 LocalPositionIn(Bounds2D parentFrame)
        {
            var topLeft = TopLeftLocalPositionIn(parentFrame);
            var result = new Vector3(
                topLeft.x + frame.size.x / 2.0f,
                topLeft.y + (frame.size.y / 2.0f) * Vector2.down.y,
                topLeft.z
            );

            return result;
        }

        public Vector2 WorldSize
        {
            get => frame.size;
        }

        public View2D()
        {
        }

        public void ApplyLayout()
        {
            foreach (Transform childTransform in gameObject.transform)
            {
                var childObject = childTransform.gameObject;
                var childView = childObject.GetComponent<View2D>();
                if (!childView) { continue; }

                var childPosition = childView.LocalPositionIn(frame);
                childObject.transform.localPosition = childPosition;
            }

            foreach (Transform childTransform in gameObject.transform)
            {
                var childObject = childTransform.gameObject;
                var childView = childObject.GetComponent<View2D>();
                if (!childView) { continue; }

                childView.ApplyLayout();
            }
        }

        public View2D RootView()
        {
            var result = this;

            while (result.transform.parent) {
                var parentView = result.transform.parent.GetComponent<View2D>();
                if (!parentView) { break; }

                result = parentView;
            }

            return result;
        }

        protected override void Start()
        {
            base.Start();

            ApplyLayout();
        }

#if UNITY_EDITOR
        protected override void OnValidate()
        {
            base.OnValidate();
            RootView().ApplyLayout();
        }

        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            EditorUtils.DrawRect(transform.position, WorldSize.x, WorldSize.y, renderState);
        }

        [CustomEditor(typeof(View2D))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Apply Layout"))
                {
                    View2D _target = (View2D)target;
                    _target.ApplyLayout();
                }
            }
        }
#endif
    }
}
