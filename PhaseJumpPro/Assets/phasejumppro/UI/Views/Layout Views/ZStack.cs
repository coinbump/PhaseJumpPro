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
    /// Arranges views in z-space stacked on top of each other
    /// </summary>
    public class ZStack : View2D
    {
        /// (EDITOR ONLY: values are moved on Awake)
        public HorizontalAlignmentType _horizontalAlignment;
        public VerticalAlignmentType _verticalAlignment;

        public Alignment alignment = new Alignment(HorizontalAlignment.center, VerticalAlignment.center);
        public float spacing = 0f;

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

        public override Optional<float> PreferredHeightWithoutConstraints(Vector2 layoutSize)
        {
            var result = 0f;
            var childViews = ChildViews();
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

            return isIntrinsic ? new(result) : null;
        }

        protected override void _ApplyLayout(Vector2 layoutSize)
        {
            var childViews = ChildViews();

            var width = layoutSize.x;
            var height = layoutSize.y;

            var preferredStackWidthOptional = PreferredWidthWithoutConstraints(layoutSize);
            var preferredStackWidth = preferredStackWidthOptional != null ? preferredStackWidthOptional.value : layoutSize.x;

            var preferredStackHeightOptional = PreferredHeightWithoutConstraints(layoutSize);
            var preferredStackHeight = preferredStackHeightOptional != null ? preferredStackHeightOptional.value : layoutSize.y;

            foreach (var view in childViews)
            {
                var frame = new Rect();

                var childPreferredWidth = view.PreferredWidthWithConstraints(layoutSize);
                var childPreferredHeight = view.PreferredWidthWithConstraints(layoutSize);

                var childWidth = childPreferredWidth != null ? childPreferredWidth.value : preferredStackWidth;
                var childHeight = childPreferredHeight != null ? childPreferredHeight.value : preferredStackHeight;

                frame.size = new Vector2(childWidth, childHeight);
                frame.origin.x = alignment.horizontalAlignment.aligner.AlignedOrigin(layoutSize.x, frame.size.x);
                frame.origin.y = alignment.verticalAlignment.aligner.AlignedOrigin(layoutSize.y, frame.size.y);

                view.Frame = frame;
            }
        }

        public override void UpdateFromSerializedProperties(bool forceUpdate)
        {
            base.UpdateFromSerializedProperties(forceUpdate);

            HorizontalAlignment horizontalAlignment = new HorizontalAlignment.Builder().HorizontalAlignmentFrom(_horizontalAlignment);
            VerticalAlignment verticalAlignment = new VerticalAlignment.Builder().VerticalAlignmentFrom(_verticalAlignment);
            alignment = new Alignment(horizontalAlignment, verticalAlignment);
        }
    }
}
