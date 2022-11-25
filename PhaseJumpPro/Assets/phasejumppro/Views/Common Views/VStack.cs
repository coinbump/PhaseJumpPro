using System;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    namespace View
    {
        public class VStack : View2D
        {
            public HorizontalAlignment alignment = HorizontalAlignment.Center;
            public float spacing = 0f;

            public override Optional<float> IntrinsicWidth {
                get
                {
                    var intrinsicWidthChildViews = FilteredChildViews((view) => view.IntrinsicWidth != null);
                    if (intrinsicWidthChildViews.Count == 0) { return base.IntrinsicWidth; }

                    var result = 0f;
                    foreach (var view in intrinsicWidthChildViews) {
                        result = Mathf.Max(result, view.IntrinsicWidth.value);
                    }

                    return new(result);
                }
                set => base.IntrinsicWidth = value;
            }

            protected override void _ApplyLayout(Bounds2D parentBounds)
            {
                var childViews = ChildViews();

                // Views with no intrinsic size
                var nonIntrinsicChildViews = new List<View2D>();

                // Views with intrinsic size
                var intrinsicHeightChildViews = new List<View2D>();
                var intrinsicWidthChildViews = new List<View2D>();

                var heightAvailable = parentBounds.size.y - (childViews.Count - 1) * spacing;
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
                        if (null != minWidth) {
                            maxIntrinsicWidth = MathF.Max(maxIntrinsicWidth, minWidth.value);
                        }
                    }
                }

                // If there are no intrinsic child views to determine width, use all available width
                if (intrinsicWidthChildViews.Count == 0)
                {
                    maxIntrinsicWidth = parentBounds.size.x;
                }

                var totalIntrinsicHeight = 0f;
                foreach (var view in intrinsicHeightChildViews) {
                    totalIntrinsicHeight += view.DefaultIntrinsicHeight;
                }

                var nonIntrinsicViewsCount = nonIntrinsicChildViews.Count;
                var nonIntrinsicTotalHeight = heightAvailable - totalIntrinsicHeight;
                var nonIntrinsicHeight = nonIntrinsicTotalHeight / nonIntrinsicViewsCount;

                var y = 0f;
                foreach (var view in childViews) {
                    var frame = view.Frame;

                    var intrinsicHeight = view.IntrinsicHeight;

                    if (null != intrinsicHeight)
                    {
                        frame.size.y = intrinsicHeight.value;
                    }
                    else
                    {
                        var height = nonIntrinsicHeight;

                        var maxHeight = view.MaxHeight;
                        if (null != maxHeight) {
                            height = MathF.Min(height, maxHeight.value);
                        }
                        var minHeight = view.MinHeight;
                        if (null != minHeight) {
                            height = MathF.Max(height, minHeight.value);
                        }

                        frame.size.y = height;
                        nonIntrinsicTotalHeight -= height;
                        nonIntrinsicViewsCount--;
                        nonIntrinsicHeight = nonIntrinsicTotalHeight / nonIntrinsicViewsCount;
                    }

                    var intrinsicWidth = view.IntrinsicWidth;
                    if (null != intrinsicWidth) {
                        frame.size.x = intrinsicWidth.value;
                    } else {
                        frame.size.x = maxIntrinsicWidth;
                    }

                    frame.origin.y = y;

                    switch (alignment) {
                        case HorizontalAlignment.Leading:
                            frame.origin.x = 0;
                            break;
                        case HorizontalAlignment.Center:
                            frame.origin.x = parentBounds.size.x / 2.0f - frame.size.x / 2.0f;
                            break;
                        case HorizontalAlignment.Trailing:
                            frame.origin.x = parentBounds.size.x - frame.size.x;
                            break;
                    }
                    
                    view.Frame = frame;

                    y += frame.size.y;
                    y += spacing;
                }
            }
        }
    }
}
