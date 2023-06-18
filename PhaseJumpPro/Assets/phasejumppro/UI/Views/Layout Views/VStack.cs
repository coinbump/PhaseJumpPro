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
    /// Arranges views vertically
    /// </summary>
    public class VStack : View2D
    {
        /// (EDITOR ONLY: values are moved on Awake)
        public HorizontalAlignmentType _alignment;

        public HorizontalAlignment alignment = HorizontalAlignment.center;
        public float spacing = 0f;

        /// <summary>
        /// Derive preferred width of stack from its child views
        /// </summary>
        public override Optional<float> PreferredWidthWithoutConstraints(Vector2 layoutSize)
        {
            var result = 0f;
            var childViews = ChildViews();
            bool isIntrinsic = false;

            foreach (var view in childViews)
            {
                var childWidth = view.PreferredWidthWithConstraints(layoutSize);
                if (null != childWidth)
                {
                    isIntrinsic = true;
                    result = Mathf.Max(result, childWidth.value);
                }
            }

            return isIntrinsic ? new(result) : null;
        }

        /// <summary>
        /// Derive preferred height of stack from its child views
        /// </summary>
        public override Optional<float> PreferredHeightWithoutConstraints(Vector2 layoutSize)
        {
            var childViews = ChildViews();
            float totalChildHeight = 0;

            // If all child views have a preferred size, we can derive the preferred height of the VStack
            foreach (var view in childViews)
            {
                var childHeight = view.PreferredHeightWithConstraints(layoutSize);
                if (null == childHeight)
                {
                    return null;
                }

                totalChildHeight += childHeight.value;
            }

            return new(totalChildHeight + (childViews.Count - 1) * spacing);
        }

        protected override void _ApplyLayout(Vector2 layoutSize)
        {
            var childViews = ChildViews();

            // Views with no intrinsic size
            var nonIntrinsicChildViews = new List<View2D>();

            // Views with intrinsic size
            var intrinsicHeightChildViews = new Dictionary<View2D, float>();

            var heightAvailable = layoutSize.y - (childViews.Count - 1) * spacing;
            foreach (var view in childViews)
            {
                var intrinsicHeight = view.PreferredHeightWithConstraints(layoutSize);
                if (null == intrinsicHeight)
                {
                    nonIntrinsicChildViews.Add(view);
                }
                else
                {
                    intrinsicHeightChildViews[view] = intrinsicHeight.value;
                }
            }

            var totalIntrinsicHeight = 0f;
            foreach (var (key, intrinsicHeight) in intrinsicHeightChildViews)
            {
                totalIntrinsicHeight += intrinsicHeight;
            }

            var nonIntrinsicViewsCount = nonIntrinsicChildViews.Count;
            var nonIntrinsicTotalHeight = heightAvailable - totalIntrinsicHeight;
            var nonIntrinsicHeight = nonIntrinsicTotalHeight / nonIntrinsicViewsCount;
            var preferredStackWidthOptional = PreferredWidthWithConstraints(layoutSize);
            var preferredStackWidth = preferredStackWidthOptional != null ? preferredStackWidthOptional.value : layoutSize.x;

            var y = 0f;
            foreach (var view in childViews)
            {
                var frame = view.Frame;

                var preferredWidth = view.PreferredWidthWithConstraints(new Vector2(preferredStackWidth, layoutSize.y));
                frame.size.x = preferredWidth != null ? preferredWidth.value : preferredStackWidth;

                if (intrinsicHeightChildViews.TryGetValue(view, out float intrinsicHeight))
                {
                    frame.size.y = intrinsicHeight;
                }
                else
                {
                    var height = view.PreferredHeightWithConstraints(new Vector2(frame.size.x, nonIntrinsicHeight));
                    frame.size.y = height != null ? height.value : nonIntrinsicHeight;
                    nonIntrinsicTotalHeight -= frame.size.y;
                    nonIntrinsicViewsCount--;
                    nonIntrinsicHeight = nonIntrinsicTotalHeight / nonIntrinsicViewsCount;
                }

                frame.origin.y = y;
                frame.origin.x = alignment.aligner.AlignedOrigin(layoutSize.x, frame.size.x);

                view.Frame = frame;

                y += frame.size.y;
                y += spacing;
            }
        }

        public override void UpdateFromSerializedProperties(bool forceUpdate)
        {
            base.UpdateFromSerializedProperties(forceUpdate);

            HorizontalAlignment horizontalAlignment = new HorizontalAlignment.Builder().HorizontalAlignmentFrom(_alignment);
            this.alignment = horizontalAlignment;
        }
    }
}
