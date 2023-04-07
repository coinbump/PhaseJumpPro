using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Arranges views horizontally
    /// </summary>
    public class HStack : View2D
    {
        // (EDITOR ONLY)
        public VerticalAlignmentType _alignment;

        public VerticalAlignment alignment = VerticalAlignment.center;
        public float spacing = 0f;

        public override Optional<float> PreferredHeightCore(Vector2 layoutSize)
        {
            var result = 0f;
            var childViews = ChildViews().Where(view => !(view is SpacerView)).ToList();
            bool isIntrinsic = false;

            foreach (var view in childViews)
            {
                var childHeight = view.PreferredHeightWithConstraints(new Vector2(layoutSize.x, layoutSize.y));
                if (null != childHeight)
                {
                    isIntrinsic = true;
                    result = Mathf.Max(result, childHeight.value);
                }
            }

            return new(isIntrinsic ? result : layoutSize.y);
        }

        protected override void _ApplyLayout(Vector2 layoutSize)
        {
            var childViews = ChildViews();

            // Views with no intrinsic size
            var nonIntrinsicChildViews = new List<View2D>();

            // Views with intrinsic size
            var intrinsicWidthChildViews = new List<View2D>();

            var widthAvailable = layoutSize.x - (childViews.Count - 1) * spacing;
            var preferredStackHeightOptional = PreferredHeightWithConstraints(layoutSize);
            var preferredStackHeight = preferredStackHeightOptional != null ? preferredStackHeightOptional.value : layoutSize.y;

            foreach (var view in childViews)
            {
                var intrinsicWidth = view.IntrinsicWidth;
                if (null == intrinsicWidth)
                {
                    nonIntrinsicChildViews.Add(view);
                }
                else
                {
                    intrinsicWidthChildViews.Add(view);
                }
            }

            var totalIntrinsicWidth = 0f;
            foreach (var view in intrinsicWidthChildViews)
            {
                totalIntrinsicWidth += view.DefaultIntrinsicWidth;
            }

            var nonIntrinsicViewsCount = nonIntrinsicChildViews.Count;
            var nonIntrinsicTotalWidth = widthAvailable - totalIntrinsicWidth;
            var nonIntrinsicWidth = nonIntrinsicTotalWidth / nonIntrinsicViewsCount;

            var x = 0f;
            foreach (var view in childViews)
            {
                var frame = view.Frame;

                var intrinsicWidth = view.IntrinsicWidth;

                if (null != intrinsicWidth)
                {
                    frame.size.x = intrinsicWidth.value;
                }
                else
                {
                    var preferredWidth = view.PreferredWidthWithConstraints(nonIntrinsicWidth);
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

        public override void UpdateFromSerializedProperties()
        {
            base.UpdateFromSerializedProperties();

            VerticalAlignment verticalAlignment = new VerticalAlignment.Builder().VerticalAlignmentFrom(_alignment);
            this.alignment = verticalAlignment;
        }
    }
}
