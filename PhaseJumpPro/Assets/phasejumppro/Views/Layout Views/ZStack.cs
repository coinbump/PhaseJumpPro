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
        // (EDITOR ONLY)
        public HorizontalAlignmentType _horizontalAlignment;
        public VerticalAlignmentType _verticalAlignment;

        public Alignment alignment = new Alignment(HorizontalAlignment.center, VerticalAlignment.center);
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

            var width = layoutSize.x;
            var height = layoutSize.y;

            foreach (var view in childViews)
            {
                var frame = new Bounds2D();

                var childWidth = view.PreferredWidth(layoutSize.x);
                var childHeight = view.PreferredHeight(new Vector2(childWidth, layoutSize.y));

                frame.size = new Vector2(childWidth, childHeight);
                frame.origin.x = alignment.horizontalAlignment.aligner.AlignedOrigin(layoutSize.x, frame.size.x);
                frame.origin.y = alignment.verticalAlignment.aligner.AlignedOrigin(layoutSize.y, frame.size.y);

                view.Frame = frame;
            }
        }

        public override void UpdateFromSerializedProperties()
        {
            base.UpdateFromSerializedProperties();

            HorizontalAlignment horizontalAlignment = new HorizontalAlignment.Builder().HorizontalAlignmentFrom(_horizontalAlignment);
            VerticalAlignment verticalAlignment = new VerticalAlignment.Builder().VerticalAlignmentFrom(_verticalAlignment);
            alignment = new Alignment(horizontalAlignment, verticalAlignment);
        }
    }
}
