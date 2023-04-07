using System;
using UnityEngine;
using System.Linq;

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
        public virtual Optional<float> PreferredWidthCore(float layoutSize)
        {
            return IntrinsicWidth;
        }

        public virtual float WidthWithConstraints(float width)
        {
            var result = width;
            var maxWidth = MaxWidth;
            if (null != maxWidth)
            {
                result = Mathf.Min(maxWidth.value, result);
            }

            var minWidth = MinWidth;
            if (null != minWidth)
            {
                result = Mathf.Max(minWidth.value, result);
            }

            return result;
        }

        public virtual Optional<float> PreferredWidthWithConstraints(float layoutSize)
        {
            var optionalResult = PreferredWidthCore(layoutSize);

            if (null != optionalResult)
            {
                return new(WidthWithConstraints(optionalResult.value));
            }

            return null;
        }

        public virtual float PreferredWidthExpanding(float layoutSize)
        {
            var optionalResult = PreferredWidthCore(layoutSize);
            float width = optionalResult != null ? optionalResult.value : layoutSize;
            return WidthWithConstraints(width);
        }

        public virtual Optional<float> PreferredHeightCore(Vector2 layoutSize)
        {
            return IntrinsicHeight;
        }

        public virtual float HeightWithConstraints(float height)
        {
            var result = height;
            var maxHeight = MaxHeight;

            if (null != maxHeight)
            {
                result = Mathf.Min(maxHeight.value, result);
            }

            var minHeight = MinHeight;
            if (null != minHeight)
            {
                result = Mathf.Max(minHeight.value, result);
            }

            return result;
        }

        public virtual Optional<float> PreferredHeightWithConstraints(Vector2 layoutSize)
        {
            var optionalResult = PreferredHeightCore(layoutSize);

            if (null != optionalResult)
            {
                return new(HeightWithConstraints(optionalResult.value));
            }

            return null;
        }

        public virtual float PreferredHeightExpanding(Vector2 layoutSize)
        {
            var optionalResult = PreferredHeightCore(layoutSize);
            float height = optionalResult != null ? optionalResult.value : layoutSize.y;
            return HeightWithConstraints(height);
        }

        public Vector3 ViewToWorldPosition(Vector2 viewPosition)
        {
            var topLeftWorldPosition = TopLeftWorldPosition(transform.position);
            var result = new Vector3(
                topLeftWorldPosition.x + viewPosition.x,
                topLeftWorldPosition.y + viewPosition.y * Vector2.down.y,
                topLeftWorldPosition.z
            );

            return result;
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

        public float ResolvedMaxWidth
        {
            get
            {
                var maxWidth = MaxWidth;
                return null != maxWidth ? maxWidth.value : float.MaxValue;
            }
        }

        protected virtual void _ApplyLayout(Vector2 layoutSize)
        {
            var childViews = ChildViews();

            // Default layout: center child views
            foreach (var view in childViews)
            {
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

            UpdateSpriteRenderer();
        }

        public virtual void SetNeedsLayout()
        {
            NeedsLayout = true;
        }

        protected virtual void _PostApplyLayout()
        {
            var childViews = ChildViews();
            foreach (var view in childViews)
            {
                view._ApplyLayout(view.Bounds.size);
                view.NeedsLayout = false;
                view._PostApplyLayout();
            }
        }

        public virtual void ApplyLayout(bool force)
        {
            // Only root view can apply layout
            var parentView = ParentView();
            if (null != parentView)
            {
                return;
            }

            if (!force && !needsLayout) { return; }

            // Debug.Log(GetType() + ": ApplyLayout");

            var parentBounds = ParentBounds();

            // Top view needs a size
            Bounds2D frame = new Bounds2D();

            var preferredWidth = PreferredWidthExpanding(parentBounds.size.x);
            var preferredHeight = PreferredHeightExpanding(new Vector2(preferredWidth, parentBounds.size.y));

            frame.size.x = preferredWidth;
            frame.size.y = preferredHeight;

            Frame = frame;

            Bounds2D layoutBounds = Bounds;

            _ApplyLayout(layoutBounds.size);
            NeedsLayout = false;
            _PostApplyLayout();

            // Subclasses can apply their own layout if needed
            UpdatePositions();
        }
    }
}
