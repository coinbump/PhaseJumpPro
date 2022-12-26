using System;
using UnityEngine;

/*
RATING: 5 stars
Utility wrapper
CODE REVIEW: 12/7/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Combines horizontal and vertical alignment
    /// </summary>
    [Serializable]
    public class Alignment
    {
        public HorizontalAlignment horizontalAlignment;
        public VerticalAlignment verticalAlignment;

        public Alignment()
        {
            this.horizontalAlignment = HorizontalAlignment.center;
            this.verticalAlignment = VerticalAlignment.center;
        }

        public Alignment(HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment)
        {
            this.horizontalAlignment = horizontalAlignment;
            this.verticalAlignment = verticalAlignment;
        }

        public static Alignment center = new Alignment(HorizontalAlignment.center, VerticalAlignment.center);
    }
}
