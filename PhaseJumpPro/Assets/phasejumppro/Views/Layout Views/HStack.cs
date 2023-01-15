using System;
using System.Collections.Generic;
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

        public override float PreferredHeight(Vector2 layoutSize)
        {
            var result = 0f;
            var childViews = ChildViews();
            foreach (var view in childViews)
            {
                // We don't care about spacers
                if (view is SpacerView) { continue; }

                var childHeight = view.PreferredHeight(layoutSize);
                result = Mathf.Max(result, childHeight);
            }

            return result;
        }

        protected override void _ApplyLayout(Vector2 layoutSize)
        {
            var childViews = ChildViews();

            // Views with no intrinsic size
            var nonIntrinsicChildViews = new List<View2D>();

            // Views with intrinsic size
            var intrinsicWidthChildViews = new List<View2D>();
            var intrinsicHeightChildViews = new List<View2D>();

            var widthAvailable = layoutSize.x - (childViews.Count - 1) * spacing;
            var maxIntrinsicHeight = 0f;
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

                var intrinsicHeight = view.IntrinsicHeight;
                if (null != intrinsicHeight)
                {
                    intrinsicHeightChildViews.Add(view);
                    maxIntrinsicHeight = MathF.Max(maxIntrinsicHeight, intrinsicHeight.value);
                }
                else
                {
                    var minHeight = view.MinHeight;
                    if (null != minHeight)
                    {
                        maxIntrinsicHeight = MathF.Max(maxIntrinsicHeight, minHeight.value);
                    }
                }
            }

            // If there are no intrinsic child views to determine height, use all available height
            if (intrinsicHeightChildViews.Count == 0)
            {
                maxIntrinsicHeight = layoutSize.y;
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
                    var width = view.PreferredWidth(nonIntrinsicWidth);
                    frame.size.x = width;
                    nonIntrinsicTotalWidth -= width;
                    nonIntrinsicViewsCount--;
                    nonIntrinsicWidth = nonIntrinsicTotalWidth / nonIntrinsicViewsCount;
                }

                var intrinsicHeight = view.IntrinsicHeight;
                if (null != intrinsicHeight)
                {
                    frame.size.y = intrinsicHeight.value;
                }
                else
                {
                    frame.size.y = maxIntrinsicHeight;
                }

                frame.origin.x = x;
                frame.origin.y = alignment.aligner.AlignedOrigin(layoutSize.y, frame.size.y);
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
