using System;
using UnityEngine;

/*
RATING: 5 stars
Simple layout view with unit tests
CODE REVIEW: 11/24/22
*/
namespace PJ
{
    namespace View
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
                    var childViews = ChildViews();
                    if (childViews.Count < 1)
                    {
                        return base.IntrinsicWidth;
                    }

                    var firstChild = childViews[0];
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
                    var childViews = ChildViews();
                    if (childViews.Count < 1)
                    {
                        return base.IntrinsicHeight;
                    }

                    var firstChild = childViews[0];
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

            protected override void _ApplyLayout(Bounds2D layoutBounds)
            {
                base._ApplyLayout(layoutBounds);

                var childViews = ChildViews();
                if (childViews.Count < 1)
                {
                    return;
                }

                var firstChild = childViews[0];
                var frame = new Bounds2D();

                var childIntrinsicWidth = firstChild.IntrinsicWidth;
                var childIntrinsicHeight = firstChild.IntrinsicHeight;

                if (null != childIntrinsicWidth) {
                    frame.size.x = childIntrinsicWidth.value;
                } else {
                    frame.size.x = layoutBounds.size.x;
                }

                if (null != childIntrinsicHeight) {
                    frame.size.y = childIntrinsicHeight.value;
                } else {
                    frame.size.y = layoutBounds.size.y;
                }

                var padLeading = PadLeading;
                frame.origin.x = padLeading;

                if (null == childIntrinsicWidth)
                {
                    frame.size.x -= padLeading;
                    frame.size.x -= PadTrailing;
                }

                var padTop = PadTop;
                frame.origin.y = padTop;

                if (null == childIntrinsicHeight)
                {
                    frame.size.y -= padTop;
                    frame.size.y -= PadBottom;
                }

                firstChild.Frame = frame;
            } // TESTED
        }
    }
}
