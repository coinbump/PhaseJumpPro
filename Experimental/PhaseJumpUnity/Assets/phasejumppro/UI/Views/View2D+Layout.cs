using System;
using UnityEngine;
using System.Linq;
using System.Collections.Generic;

/*
RATING: 5 stars
Has some unit tests
CODE REVIEW: 6/11/23
*/
namespace PJ
{
    public enum LayoutQueryOption
    {
        // Send back value with constraints (min, max)
        WithConstraints
    }

    [Serializable]
    public partial class View2D
    {
        public bool IsFixedWidth
        {
            get => isFixedWidth || FixedWidth != null;
            set => isFixedWidth = value;
        }
        public bool IsFixedHeight
        {
            get => isFixedHeight || FixedHeight != null;
            set => isFixedHeight = value;
        }

        /// <summary>
        /// Returns the ideal width for this view, ignoring constraints
        /// </summary>
        public virtual Optional<float> PreferredWidthWithoutConstraints(Vector2 layoutSize)
        {
            if (IsFixedWidth)
            {
                var result = IntrinsicWidth;
                if (null != result) { return result; }

                var firstChild = FirstChildView;
                if (firstChild)
                {
                    return firstChild.PreferredWidthWithConstraints(layoutSize);
                }
            }

            return null;
        } // TESTED

        /// <summary>
        /// Applies the view size constraints to the proposed width
        /// </summary>
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
        } // TESTED

        /// <summary>
        /// Applies the size constraints to the preferred width
        /// </summary>
        public virtual Optional<float> PreferredWidthWithConstraints(Vector2 layoutSize)
        {
            var optionalResult = PreferredWidthWithoutConstraints(layoutSize);

            if (null != optionalResult)
            {
                return new(WidthWithConstraints(optionalResult.value));
            }

            return null;
        } // TESTED

        /// <summary>
        /// If the preferred size is null, expand to the layout size, and optionally apply constraints
        /// </summary>
        public virtual float PreferredWidthLayoutQuery(Vector2 layoutSize, List<LayoutQueryOption> options)
        {
            var optionalResult = PreferredWidthWithoutConstraints(layoutSize);
            float width = optionalResult != null ? optionalResult.value : layoutSize.x;
            return options.Contains(LayoutQueryOption.WithConstraints) ? WidthWithConstraints(width) : width;
        } // TESTED

        /// <summary>
        /// Returns the ideal height for this view, ignoring constraints
        /// </summary>
        public virtual Optional<float> PreferredHeightWithoutConstraints(Vector2 layoutSize)
        {
            if (IsFixedHeight)
            {
                var result = IntrinsicHeight;
                if (null != result) { return result; }

                var firstChild = FirstChildView;
                if (firstChild)
                {
                    return firstChild.PreferredHeightWithConstraints(layoutSize);
                }
            }

            return null;
        }

        /// <summary>
        /// Applies the view size constraints to the proposed height
        /// </summary>
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
        } // TESTED

        /// <summary>
        /// Applies the size constraints to the preferred height
        /// </summary>
        public virtual Optional<float> PreferredHeightWithConstraints(Vector2 layoutSize)
        {
            var optionalResult = PreferredHeightWithoutConstraints(layoutSize);

            if (null != optionalResult)
            {
                return new(HeightWithConstraints(optionalResult.value));
            }

            return null;
        } // TESTED

        /// <summary>
        /// If the preferred size is null, expand to the layout size, and optionally apply constraints
        /// </summary>
        public virtual float PreferredHeightLayoutQuery(Vector2 layoutSize, List<LayoutQueryOption> options)
        {
            var optionalResult = PreferredHeightWithoutConstraints(layoutSize);
            float height = optionalResult != null ? optionalResult.value : layoutSize.y;
            return options.Contains(LayoutQueryOption.WithConstraints) ? HeightWithConstraints(height) : height;
        } // TESTED

        public Vector3 ViewToWorldPosition(Vector2 viewPosition)
        {
            var topLeftWorldPosition = TopLeftWorldPosition(transform.position);
            var result = new Vector3(
                topLeftWorldPosition.x + viewPosition.x,
                topLeftWorldPosition.y + viewPosition.y * Vector2.down.y,
                topLeftWorldPosition.z
            );

            return result;
        } // TESTED

        public Vector2 WorldToViewPosition(Vector3 worldPosition)
        {
            var localPosition = transform.InverseTransformPoint(worldPosition);
            return LocalToViewPosition(localPosition);
        } // TESTED

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

        public Vector3 TopLeftLocalPositionIn(Rect parentFrame, Vector3 transformLocalPosition)
        {
            var topLeft = new Vector3(
                -parentFrame.size.x / 2.0f + frame.origin.x,
                parentFrame.size.y / 2.0f * Vector2.up.y + frame.origin.y * Vector2.down.y,
                0
            );
            return topLeft;
        } // TESTED

        public Vector3 LocalPositionIn(Rect parentFrame, Vector3 transformLocalPosition)
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

        public Optional<float> FixedWidth
        {
            get => tags.Value<float>("width.fixed");
            set => tags.SetOptionalValue("width.fixed", value);
        }
        public Optional<float> FixedHeight
        {
            get => tags.Value<float>("height.fixed");
            set => tags.SetOptionalValue("height.fixed", value);
        }
        public Optional<float> MinWidth
        {
            get => tags.Value<float>("width.min");
            set => tags.SetOptionalValue("width.min", value);
        }
        public Optional<float> MinHeight
        {
            get => tags.Value<float>("height.min");
            set => tags.SetOptionalValue("height.min", value);
        }
        public Optional<float> MaxWidth
        {
            get => tags.Value<float>("width.max");
            set => tags.SetOptionalValue("width.max", value);
        }
        public Optional<float> MaxHeight
        {
            get => tags.Value<float>("height.max");
            set => tags.SetOptionalValue("height.max", value);
        }

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
                var preferredWidth = view.PreferredWidthWithConstraints(layoutSize);
                var preferredHeight = view.PreferredHeightWithConstraints(layoutSize);

                var width = preferredWidth != null ? preferredWidth.value : layoutSize.x;
                var height = preferredHeight != null ? preferredHeight.value : layoutSize.y;

                var origin = new Vector2(
                    layoutSize.x / 2.0f - (width / 2.0f),
                    layoutSize.y / 2.0f - (height / 2.0f)
                );
                var size = new Vector2(width, height);
                view.Frame = new Rect(origin, size);
            }
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
            var parentView = ParentView;
            if (null != parentView)
            {
                return;
            }

            if (!force && !needsLayout) { return; }

            // Debug.Log(GetType() + ": ApplyLayout");

            var parentBounds = ParentBounds();

            // Top view needs a size
            Rect frame = new Rect();

            var preferredWidth = PreferredWidthLayoutQuery(parentBounds.size, new() { LayoutQueryOption.WithConstraints });
            var preferredHeight = PreferredHeightLayoutQuery(new Vector2(preferredWidth, parentBounds.size.y), new() { LayoutQueryOption.WithConstraints });

            frame.size.x = preferredWidth;
            frame.size.y = preferredHeight;

            Frame = frame;

            Rect layoutBounds = Bounds;

            _ApplyLayout(layoutBounds.size);
            NeedsLayout = false;
            _PostApplyLayout();

            // Subclasses can apply their own layout if needed
            UpdatePositions();
        }
    }
}
