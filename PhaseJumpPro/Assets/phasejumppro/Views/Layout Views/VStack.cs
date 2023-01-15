using System;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Arranges views vertically
    /// </summary>
    public class VStack : View2D
    {
        // (EDITOR ONLY)
        public HorizontalAlignmentType _alignment;

        public HorizontalAlignment alignment = HorizontalAlignment.center;
        public float spacing = 0f;

        public override float PreferredWidth(float layoutSize)
        {
            var result = 0f;
            var childViews = ChildViews();
            foreach (var view in childViews)
            {
                // We don't care about spacers
                if (view is SpacerView) { continue; }

                var childWidth = view.PreferredWidth(layoutSize);
                result = Mathf.Max(result, childWidth);
            }

            return result;
        }

        protected override void _ApplyLayout(Vector2 layoutSize)
        {
            var childViews = ChildViews();

            // Views with no intrinsic size
            var nonIntrinsicChildViews = new List<View2D>();

            // Views with intrinsic size
            var intrinsicHeightChildViews = new List<View2D>();
            var intrinsicWidthChildViews = new List<View2D>();

            var heightAvailable = layoutSize.y - (childViews.Count - 1) * spacing;
            var maxIntrinsicWidth = 0f;
            foreach (var view in childViews)
            {
                var intrinsicHeight = view.IntrinsicHeight;
                if (null == intrinsicHeight)
                {
                    nonIntrinsicChildViews.Add(view);
                }
                else
                {
                    intrinsicHeightChildViews.Add(view);
                }

                var intrinsicWidth = view.IntrinsicWidth;
                if (null != intrinsicWidth)
                {
                    intrinsicWidthChildViews.Add(view);
                    maxIntrinsicWidth = MathF.Max(maxIntrinsicWidth, intrinsicWidth.value);
                }
                else
                {
                    var minWidth = view.MinWidth;
                    if (null != minWidth)
                    {
                        maxIntrinsicWidth = MathF.Max(maxIntrinsicWidth, minWidth.value);
                    }
                }
            }

            // If there are no intrinsic child views to determine width, use all available width
            if (intrinsicWidthChildViews.Count == 0)
            {
                maxIntrinsicWidth = layoutSize.x;
            }

            var totalIntrinsicHeight = 0f;
            foreach (var view in intrinsicHeightChildViews)
            {
                totalIntrinsicHeight += view.DefaultIntrinsicHeight;
            }

            var nonIntrinsicViewsCount = nonIntrinsicChildViews.Count;
            var nonIntrinsicTotalHeight = heightAvailable - totalIntrinsicHeight;
            var nonIntrinsicHeight = nonIntrinsicTotalHeight / nonIntrinsicViewsCount;

            var y = 0f;
            foreach (var view in childViews)
            {
                var frame = view.Frame;

                var intrinsicHeight = view.IntrinsicHeight;

                var intrinsicWidth = view.IntrinsicWidth;
                if (null != intrinsicWidth)
                {
                    frame.size.x = intrinsicWidth.value;
                }
                else
                {
                    frame.size.x = maxIntrinsicWidth;
                }

                if (null != intrinsicHeight)
                {
                    frame.size.y = intrinsicHeight.value;
                }
                else
                {
                    var height = view.PreferredHeight(new Vector2(frame.size.x, nonIntrinsicHeight));
                    frame.size.y = height;
                    nonIntrinsicTotalHeight -= height;
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

        public override void UpdateFromSerializedProperties()
        {
            base.UpdateFromSerializedProperties();

            HorizontalAlignment horizontalAlignment = new HorizontalAlignment.Builder().HorizontalAlignmentFrom(_alignment);
            this.alignment = horizontalAlignment;
        }
    }
}
