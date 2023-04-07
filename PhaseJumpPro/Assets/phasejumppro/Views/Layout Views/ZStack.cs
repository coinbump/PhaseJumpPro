using System;
using System.Collections.Generic;
using System.Linq;
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

        public override Optional<float> PreferredWidthCore(float layoutSize)
        {
            var result = 0f;
            var childViews = ChildViews().Where(view => !(view is SpacerView)).ToList();
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

            return new(isIntrinsic ? result : layoutSize);
        }

        public override Optional<float> PreferredHeightCore(Vector2 layoutSize)
        {
            var result = 0f;
            var childViews = ChildViews().Where(view => !(view is SpacerView)).ToList();
            var isIntrinsic = false;

            foreach (var view in childViews)
            {
                var childHeight = view.PreferredHeightWithConstraints(layoutSize);
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

            var width = layoutSize.x;
            var height = layoutSize.y;
            var preferredStackHeightOptional = PreferredHeightCore(layoutSize);
            var preferredStackHeight = preferredStackHeightOptional != null ? preferredStackHeightOptional.value : layoutSize.y;

            foreach (var view in childViews)
            {
                var frame = new Bounds2D();

                var childPreferredWidth = view.PreferredWidthWithConstraints(layoutSize.x);
                var childWidth = childPreferredWidth != null ? childPreferredWidth.value : layoutSize.x;
                var childHeight = view.PreferredHeightWithConstraints(new Vector2(childWidth, layoutSize.y));

                frame.size = new Vector2(childWidth, childHeight != null ? childHeight.value : preferredStackHeight);
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
