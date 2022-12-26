using System;
using UnityEngine;

/*
RATING: 5 stars
Simple layout view with unit tests
CODE REVIEW: 11/24/22
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

        public override Optional<float> IntrinsicWidth
        {
            get
            {
                var firstChild = FirstChildView();
                if (null == firstChild) { return base.IntrinsicWidth; }

                var childIntrinsicWidth = firstChild.IntrinsicWidth;
                if (null == childIntrinsicWidth) { return null; }
                
                var result = childIntrinsicWidth.value;

                result += PadLeading;
                result += PadTrailing;

                return new(result);
            }
            set {
                intrinsicWidth = value;
            }
        } // TESTED

        public override Optional<float> IntrinsicHeight
        {
            get
            {
                var firstChild = FirstChildView();
                if (null == firstChild) { return base.IntrinsicHeight; }

                var childIntrinsicHeight = firstChild.IntrinsicHeight;
                if (null != childIntrinsicHeight)
                {
                    var result = childIntrinsicHeight.value;

                    result += PadTop;
                    result += PadBottom;

                    return new(result);
                }
                return null;
            }
            set {
                intrinsicHeight = value;
            }
        } // TESTED

        protected override void _ApplyLayout(Vector2 layoutSize)
        {
            var firstChild = FirstChildView();
            if (null == firstChild) { return; }

            var frame = new Bounds2D();

            var childPreferredWidth = firstChild.PreferredWidth(layoutSize.x - PadLeading - PadTrailing);
            var childPreferredHeight = firstChild.PreferredHeight(layoutSize.y - PadTop - PadBottom);

            frame.size.x = childPreferredWidth;
            frame.size.y = childPreferredHeight;
            frame.origin.x = PadLeading;
            frame.origin.y = PadTop;

            firstChild.Frame = frame;
        } // TESTED
    }
}
