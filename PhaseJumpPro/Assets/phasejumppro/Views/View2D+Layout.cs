using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 11/14/22
 */
namespace PJ
{
    /// <summary>
    /// Testable utility class for View2D coordinate conversions
    /// </summary>
    [Serializable]
    public partial class View2D
    {
        public virtual float PreferredWidth(float layoutSize)
        {
            var optionalResult = IntrinsicWidth;

            if (null == optionalResult)
            {
                var result = layoutSize;
                var maxWidth = MaxWidth;
                if (null != maxWidth) {
                    result = Mathf.Min(maxWidth.value, result);
                }
                
                var minWidth = MinWidth;
                if (null != minWidth) {
                    result = Mathf.Max(minWidth.value, result);
                }

                return result;
            }

            return optionalResult.value;
        }

        public virtual float PreferredHeight(float layoutSize)
        {
            var optionalResult = IntrinsicHeight;

            if (null == optionalResult)
            {
                var result = layoutSize;
                var maxHeight = MaxHeight;
                if (null != maxHeight) {
                    result = Mathf.Min(maxHeight.value, result);
                }
                
                var minHeight = MinHeight;
                if (null != minHeight) {
                    result = Mathf.Max(minHeight.value, result);
                }

                return result;
            }

            return optionalResult.value;
        }

        public Vector3 WorldToViewPosition(Vector3 worldPosition)
        {
            var localPosition = transform.InverseTransformPoint(worldPosition);
            return LocalToViewPosition(localPosition);
        }

        public Vector3 LocalToViewPosition(Vector3 localPosition)
        {
            var viewX = localPosition.x + frame.size.x / 2.0f;
            var viewY = frame.size.y / 2.0f - localPosition.y;

            return new Vector3(viewX, viewY, 0);
        } // TESTED

        public Vector3 TopLeftWorldPosition(Vector3 transformWorldPosition)
        {
            var topLeft = new Vector3(
                transformWorldPosition.x - WorldSize.x / 2.0f,
                transformWorldPosition.y + WorldSize.y / 2.0f * Vector2.up.y,
                transformWorldPosition.z
            );
            return topLeft;
        } // TESTED

        public Vector3 TopLeftLocalPositionIn(Bounds2D parentFrame, Vector3 transformLocalPosition)
        {
            var topLeft = new Vector3(
                -parentFrame.size.x / 2.0f + frame.origin.x,
                parentFrame.size.y / 2.0f * Vector2.up.y + frame.origin.y * Vector2.down.y,
                0
            );
            return topLeft;
        } // TESTED

        public Vector3 LocalPositionIn(Bounds2D parentFrame, Vector3 transformLocalPosition)
        {
            var topLeft = TopLeftLocalPositionIn(parentFrame, transformLocalPosition);
            var result = new Vector3(
                topLeft.x + frame.size.x / 2.0f,
                topLeft.y + (frame.size.y / 2.0f) * Vector2.down.y,
                0
            );

            return result;
        } // TESTED

        public bool IsViewPositionInside(Vector3 viewPosition)
        {
            if (viewPosition.x < 0 || viewPosition.y < 0) { return false; }
            if (viewPosition.x > WorldSize.x || viewPosition.y > WorldSize.y) { return false; }

            return true;
        } // TESTED

        public Optional<float> FixedWidth => tags.Value<float>("width.fixed");
        public Optional<float> FixedHeight => tags.Value<float>("height.fixed");
        public Optional<float> MinWidth => tags.Value<float>("width.min");
        public Optional<float> MinHeight => tags.Value<float>("height.min");
        public Optional<float> MaxWidth => tags.Value<float>("width.max");
        public Optional<float> MaxHeight => tags.Value<float>("height.max");

        protected virtual void _ApplyLayout(Vector2 layoutSize) {
            var childViews = ChildViews();

            // Default layout: center child views
            foreach (var view in childViews) {
                var intrinsicWidth = view.IntrinsicWidth;
                var intrinsicHeight = view.IntrinsicHeight;

                var width = intrinsicWidth != null ? intrinsicWidth.value : layoutSize.x;
                var height = intrinsicHeight != null ? intrinsicHeight.value : layoutSize.y;

                var origin = new Vector2(
                    layoutSize.x / 2.0f - (width / 2.0f),
                    layoutSize.y / 2.0f - (height / 2.0f)
                );
                var size = new Vector2(width, height);
                view.Frame = new Bounds2D(origin, size);
            }
        }

        protected virtual void _PostApplyLayout() {
            var childViews = ChildViews();
            foreach (var view in childViews) {
                view._ApplyLayout(view.Bounds.size);
                view._PostApplyLayout();
            }
        }

        public virtual void ApplyLayout() {
            Debug.Log(GetType() + ": ApplyLayout");

            if (null == ParentView()) {
                var parentBounds = ParentBounds();

                // Top view needs a size
                Bounds2D frame = new Bounds2D();

                var preferredWidth = PreferredWidth(parentBounds.size.x);
                var preferredHeight = PreferredHeight(parentBounds.size.y);

                frame.size.x = preferredWidth;
                frame.size.y = preferredHeight;

                Frame = frame;
            }

            Bounds2D layoutBounds = Bounds;

            _ApplyLayout(layoutBounds.size);
            _PostApplyLayout();

            // Subclasses can apply their own layout if needed
            UpdatePositions();
        }
    }
}
