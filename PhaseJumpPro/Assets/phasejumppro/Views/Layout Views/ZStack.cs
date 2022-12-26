using System;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Arranges views in z-space stacked on top of each other
    /// </summary>
    public class ZStack : View2D
    {
        public Alignment alignment = new Alignment(HorizontalAlignment.center, VerticalAlignment.center);
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

        protected override void _ApplyLayout(Vector2 layoutSize)
        {
            var childViews = ChildViews();

            var width = layoutSize.x;
            var height = layoutSize.y;

            foreach (var view in childViews) {
                var frame = new Bounds2D();

                var childWidth = view.PreferredWidth(layoutSize.x);
                var childHeight = view.PreferredHeight(layoutSize.y);

                frame.size = new Vector2(childWidth, childHeight);
                frame.origin.x = alignment.horizontalAlignment.aligner.AlignedOrigin(layoutSize.x, frame.size.x);
                frame.origin.y = alignment.verticalAlignment.aligner.AlignedOrigin(layoutSize.y, frame.size.y);

                view.Frame = frame;
            }
        }
    }
}
