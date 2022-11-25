using System;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    namespace View
    {
        public class HStack : View2D
        {
            public VerticalAlignment alignment = VerticalAlignment.Center;
            public float spacing = 0f;

            public override Optional<float> IntrinsicHeight {
                get
                {
                    var intrinsicHeightChildViews = FilteredChildViews((view) => view.IntrinsicHeight != null);
                    if (intrinsicHeightChildViews.Count == 0) { return base.IntrinsicHeight; }

                    var result = 0f;
                    foreach (var view in intrinsicHeightChildViews) {
                        result = Mathf.Max(result, view.IntrinsicHeight.value);
                    }

                    return new(result);
                }
                set => base.IntrinsicHeight = value;
            }

            protected override void _ApplyLayout(Bounds2D parentBounds)
            {
                var childViews = ChildViews();

                // Views with no intrinsic size
                var nonIntrinsicChildViews = new List<View2D>();

                // Views with intrinsic size
                var intrinsicWidthChildViews = new List<View2D>();
                var intrinsicHeightChildViews = new List<View2D>();

                var widthAvailable = parentBounds.size.x - (childViews.Count - 1) * spacing;
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
                        if (null != minHeight) {
                            maxIntrinsicHeight = MathF.Max(maxIntrinsicHeight, minHeight.value);
                        }
                    }
                }

                // If there are no intrinsic child views to determine height, use all available height
                if (intrinsicHeightChildViews.Count == 0)
                {
                    maxIntrinsicHeight = parentBounds.size.y;
                }

                var totalIntrinsicWidth = 0f;
                foreach (var view in intrinsicWidthChildViews) {
                    totalIntrinsicWidth += view.DefaultIntrinsicWidth;
                }

                var nonIntrinsicViewsCount = nonIntrinsicChildViews.Count;
                var nonIntrinsicTotalWidth = widthAvailable - totalIntrinsicWidth;
                var nonIntrinsicWidth = nonIntrinsicTotalWidth / nonIntrinsicViewsCount;

                var x = 0f;
                foreach (var view in childViews) {
                    var frame = view.Frame;

                    var intrinsicWidth = view.IntrinsicWidth;

                    if (null != intrinsicWidth)
                    {
                        frame.size.x = intrinsicWidth.value;
                    }
                    else
                    {
                        var width = nonIntrinsicWidth;

                        var maxWidth = view.MaxWidth;
                        if (null != maxWidth) {
                            width = MathF.Min(width, maxWidth.value);
                        }
                        var minWidth = view.MinWidth;
                        if (null != minWidth) {
                            width = MathF.Max(width, minWidth.value);
                        }

                        frame.size.x = width;
                        nonIntrinsicTotalWidth -= width;
                        nonIntrinsicViewsCount--;
                        nonIntrinsicWidth = nonIntrinsicTotalWidth / nonIntrinsicViewsCount;
                    }

                    var intrinsicHeight = view.IntrinsicHeight;
                    if (null != intrinsicHeight) {
                        frame.size.y = intrinsicHeight.value;
                    } else {
                        frame.size.y = maxIntrinsicHeight;
                    }

                    frame.origin.x = x;

                    switch (alignment) {
                        case VerticalAlignment.Top:
                            frame.origin.y = 0;
                            break;
                        case VerticalAlignment.Center:
                            frame.origin.y = parentBounds.size.y / 2.0f - frame.size.y / 2.0f;
                            break;
                        case VerticalAlignment.Bottom:
                            frame.origin.y = parentBounds.size.y - frame.size.y;
                            break;
                    }
                    
                    view.Frame = frame;

                    x += frame.size.x;
                    x += spacing;
                }
            }
        }
    }
}
