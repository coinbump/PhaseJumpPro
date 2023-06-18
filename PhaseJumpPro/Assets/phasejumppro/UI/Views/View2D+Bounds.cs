using System;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 6/10/23
*/
namespace PJ
{
    public partial class View2D
    {
        public Rect Frame
        {
            get => frame;
            set
            {
                if (frame.Equals(value)) { return; }
                frame = value;
                OnFrameChange();
            }
        } // TESTED

        /// <summary>
        /// Local bounds (origin is always zero)
        /// </summary>
        public Rect Bounds
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
            OnViewSizeChange();
        }

        protected virtual void UpdateFrameComponents()
        {
            var frameSize = Frame.size;
            var unscaledFrameSize = frameSize;

            // Make sure our collider fits the view size
            if (TryGetComponent(out BoxCollider2D boxCollider))
            {
                boxCollider.offset = Vector2.zero;
                boxCollider.size = unscaledFrameSize;
            }

            if (TryGetComponent(out CapsuleCollider2D capsuleCollider))
            {
                capsuleCollider.offset = Vector2.zero;
                capsuleCollider.size = unscaledFrameSize;
            }

            if (TryGetComponent(out CircleCollider2D circleCollider))
            {
                circleCollider.offset = Vector2.zero;
                circleCollider.radius = Mathf.Max(unscaledFrameSize.x, unscaledFrameSize.y) / 2.0f;
            }

            var worldSizables = GetComponents<WorldSizeAble2D>();
            foreach (var worldSizable in worldSizables)
            {
                worldSizable.WorldSize2D = unscaledFrameSize;
            }
        }

        public virtual Rect ParentBounds()
        {
            var parent = ParentView;
            var parentBounds = new Rect();
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
