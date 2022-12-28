using System;
using UnityEngine;

namespace PJ
{
    public partial class View2D
    {
        public Bounds2D Frame
        {
            get => frame;
            set
            {
                if (frame.Equals(value)) { return; }
                frame = value;
                OnFrameChange();
            }
        }

        /// <summary>
        /// Local bounds (origin is always zero)
        /// </summary>
        public Bounds2D Bounds
        {
            get
            {
                var result = Frame;
                result.origin = Vector2.zero;
                return result;
            }
        }

        public Vector2 WorldSize
        {
            get => Frame.size;
            set
            {
                var newFrame = Frame;
                newFrame.size = value;
                Frame = newFrame;
            }
        } // TESTED

        protected virtual void OnFrameChange()
        {
            UpdateFrameComponents();
        }

        protected virtual void UpdateFrameComponents()
        {
            // Make sure our collider fits the view size
            if (TryGetComponent(out BoxCollider2D boxCollider))
            {
                boxCollider.size = Frame.size;
            }
        }

        public Bounds2D ParentBounds()
        {
            var parent = ParentView();
            var parentBounds = new Bounds2D();
            if (parent)
            {
                parentBounds = parent.Bounds;
            }
            else
            {
                var screenWorldSize = Utils.ScreenWorldSize(Camera);
                // Debug.Log("ScreenWorldSize: " + screenWorldSize.ToString());
                parentBounds.size = screenWorldSize;
            }

            return parentBounds;
        }
    }
}
