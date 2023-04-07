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
            UpdateSpriteRenderer();
        }

        protected virtual void UpdateFrameComponents()
        {
            // Frame is adjusted for scale, but the components are not, so reverse the scale
            var framesize = Frame.size;
            var unscaledFrameSize = Frame.size / new Vector2(transform.localScale.x, transform.localScale.y);

            // Make sure our collider fits the view size
            if (TryGetComponent(out BoxCollider2D boxCollider))
            {
                boxCollider.size = unscaledFrameSize;
            }

            // Make sure our collider fits the view size
            if (TryGetComponent(out CapsuleCollider2D capsuleCollider))
            {
                capsuleCollider.size = unscaledFrameSize;
            }

            var worldSizables = GetComponents<WorldSizeAble2D>();
            foreach (var worldSizable in worldSizables)
            {
                worldSizable.WorldSize = unscaledFrameSize;
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
