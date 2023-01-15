using System;
using UnityEngine;

/*
RATING: 5 stars
Has unit tests
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

        public override float PreferredWidth(float layoutSize)
        {
            var firstChild = FirstChildView();
            if (null == firstChild) { return base.PreferredWidth(layoutSize); }

            var preferredHeight = firstChild.PreferredWidth(layoutSize);
            var result = preferredHeight;

            result += PadLeading;
            result += PadTrailing;

            return result;
        }

        public override float PreferredHeight(Vector2 layoutSize)
        {
            var firstChild = FirstChildView();
            if (null == firstChild) { return base.PreferredHeight(layoutSize); }

            var preferredHeight = firstChild.PreferredHeight(layoutSize);
            var result = preferredHeight;

            result += PadTop;
            result += PadBottom;

            return result;
        }

        protected override void _ApplyLayout(Vector2 layoutSize)
        {
            var firstChild = FirstChildView();
            if (null == firstChild) { return; }

            var frame = new Bounds2D();

            var layoutSizeX = layoutSize.x - PadLeading - PadTrailing;
            var childPreferredWidth = firstChild.PreferredWidth(layoutSizeX);
            var childPreferredHeight = firstChild.PreferredHeight(new Vector2(childPreferredWidth, layoutSize.y - PadTop - PadBottom));

            frame.size.x = childPreferredWidth;
            frame.size.y = childPreferredHeight;
            frame.origin.x = PadLeading;
            frame.origin.y = PadTop;

            firstChild.Frame = frame;
        } // TESTED
    }
}
