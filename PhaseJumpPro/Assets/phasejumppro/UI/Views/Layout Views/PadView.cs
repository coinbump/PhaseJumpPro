using System;
using UnityEngine;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 6/20/23
*/
namespace PJ
{
    /// <summary>
    /// View that adds intrinsic padding to child
    /// </summary>
    public class PadView : View2D
    {
        [Header("PadView Properties")]
        public float padLeading;
        public float padTrailing;
        public float padTop;
        public float padBottom;

        public float PadLeading => padLeading;
        public float PadTrailing => padTrailing;
        public float PadTop => padTop;
        public float PadBottom => padBottom;

        public override Optional<float> PreferredWidthWithoutConstraints(Vector2 layoutSize)
        {
            var firstChild = FirstChildView;
            if (null == firstChild) { return base.PreferredWidthWithoutConstraints(layoutSize); }

            var preferredWidth = firstChild.PreferredWidthWithConstraints(layoutSize);
            var result = preferredWidth != null ? preferredWidth.value : 0;

            result += PadLeading;
            result += PadTrailing;

            return new(result);
        }

        public override Optional<float> PreferredHeightWithoutConstraints(Vector2 layoutSize)
        {
            var firstChild = FirstChildView;
            if (null == firstChild) { return base.PreferredHeightWithoutConstraints(layoutSize); }

            var preferredHeight = firstChild.PreferredHeightWithConstraints(layoutSize);
            var result = preferredHeight != null ? preferredHeight.value : 0;

            result += PadTop;
            result += PadBottom;

            return new(result);
        }

        protected override void _ApplyLayout(Vector2 layoutSize)
        {
            var firstChild = FirstChildView;
            if (null == firstChild) { return; }

            var frame = new Rect();

            var layoutSizeX = layoutSize.x - PadLeading - PadTrailing;
            var childPreferredWidth = firstChild.PreferredWidthWithConstraints(new Vector2(layoutSizeX, layoutSize.y));
            frame.size.x = childPreferredWidth != null ? childPreferredWidth.value : 0;

            var childPreferredHeight = firstChild.PreferredHeightWithConstraints(new Vector2(frame.size.x, layoutSize.y - PadTop - PadBottom));

            frame.size.y = childPreferredHeight != null ? childPreferredHeight.value : 0;
            frame.origin.x = PadLeading;
            frame.origin.y = PadTop;

            firstChild.Frame = frame;
        } // TESTED
    }
}
