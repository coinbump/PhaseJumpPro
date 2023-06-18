using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 6/11/23
*/
namespace PJ
{
    /// <summary>
    /// Arranges views horizontally
    /// </summary>
    public class HStack : View2D
    {
        /// (EDITOR ONLY: values are moved on Awake)
        public VerticalAlignmentType _alignment;

        public VerticalAlignment alignment = VerticalAlignment.center;
        public float spacing = 0f;

        /// <summary>
        /// Derive preferred width of stack from its child views
        /// </summary>
        public override Optional<float> PreferredWidthWithoutConstraints(Vector2 layoutSize)
        {
            var childViews = ChildViews();
            float totalChildWidth = 0;

            // If all child views have a preferred size, we can derive the preferred width of the HStack
            foreach (var view in childViews)
            {
                var childWidth = view.PreferredWidthWithConstraints(layoutSize);
                if (null == childWidth)
                {
                    return null;
                }

                totalChildWidth += childWidth.value;
            }

            return new(totalChildWidth + (childViews.Count - 1) * spacing);
        }

        /// <summary>
        /// Derive preferred height of stack from its child views
        /// </summary>
        public override Optional<float> PreferredHeightWithoutConstraints(Vector2 layoutSize)
        {
            var result = 0f;
            var childViews = ChildViews();
            bool isIntrinsic = false;

            foreach (var view in childViews)
            {
                var childHeight = view.PreferredHeightWithConstraints(layoutSize);
                if (null != childHeight)
                {
                    isIntrinsic = true;
                    result = Mathf.Max(result, childHeight.value);
                }
            }

            return isIntrinsic ? new(result) : null;
        }

        protected override void _ApplyLayout(Vector2 layoutSize)
        {
            var childViews = ChildViews();

            // Views with no intrinsic size
            var nonIntrinsicChildViews = new List<View2D>();

            // Views with intrinsic size
            var intrinsicWidthChildViews = new Dictionary<View2D, float>();

            var widthAvailable = layoutSize.x - (childViews.Count - 1) * spacing;
            var preferredStackHeightOptional = PreferredHeightWithConstraints(layoutSize);
            var preferredStackHeight = preferredStackHeightOptional != null ? preferredStackHeightOptional.value : layoutSize.y;

            foreach (var view in childViews)
            {
                var intrinsicWidth = view.PreferredWidthWithConstraints(layoutSize);
                if (null == intrinsicWidth)
                {
                    nonIntrinsicChildViews.Add(view);
                }
                else
                {
                    intrinsicWidthChildViews[view] = intrinsicWidth.value;
                }
            }

            var totalIntrinsicWidth = 0f;
            foreach (var (key, intrinsicWidth) in intrinsicWidthChildViews)
            {
                totalIntrinsicWidth += intrinsicWidth;
            }

            var nonIntrinsicViewsCount = nonIntrinsicChildViews.Count;
            var nonIntrinsicTotalWidth = widthAvailable - totalIntrinsicWidth;
            var nonIntrinsicWidth = nonIntrinsicTotalWidth / nonIntrinsicViewsCount;

            var x = 0f;
            foreach (var view in childViews)
            {
                var frame = view.Frame;

                if (intrinsicWidthChildViews.TryGetValue(view, out float intrinsicWidth))
                {
                    frame.size.x = intrinsicWidth;
                }
                else
                {
                    var preferredWidth = view.PreferredWidthWithConstraints(new(nonIntrinsicWidth, layoutSize.y));
                    var width = preferredWidth != null ? preferredWidth.value : nonIntrinsicWidth;
                    frame.size.x = width;
                    nonIntrinsicTotalWidth -= width;
                    nonIntrinsicViewsCount--;
                    nonIntrinsicWidth = nonIntrinsicTotalWidth / nonIntrinsicViewsCount;
                }

                var preferredHeight = view.PreferredHeightWithConstraints(new Vector2(frame.size.x, preferredStackHeight));
                frame.size.y = preferredHeight != null ? preferredHeight.value : preferredStackHeight;

                frame.origin.x = x;
                frame.origin.y = alignment.aligner.AlignedOrigin(preferredStackHeight, frame.size.y);
                view.Frame = frame;

                x += frame.size.x;
                x += spacing;
            }
        }

        public override void UpdateFromSerializedProperties(bool forceUpdate)
        {
            base.UpdateFromSerializedProperties(forceUpdate);

            VerticalAlignment verticalAlignment = new VerticalAlignment.Builder().VerticalAlignmentFrom(_alignment);
            this.alignment = verticalAlignment;
        }
    }
}
