using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Values model
CODE REVIEW: 11/30/22
*/

namespace PJ
{
    [Serializable]
    public struct GridViewItem
    {
        public enum SizeType
        {
            // Fixed size column/row, based on .size
            Fixed,

            // FUTURE: To support flexible with min-max, we need a constraints solver
            // Fits available width
            Flexible
        }

        public SizeType sizeType;
        public float size;
        public Alignment alignment;
        public float spacing;

        // FUTURE: for flexible (requires constraints solver)
        // public float minSize;
        // public float maxSize;

        public GridViewItem(SizeType sizeType, float size, Alignment alignment, float spacing = 0f)
        {
            this.sizeType = sizeType;
            this.size = size;
            this.spacing = spacing;
            this.alignment = alignment;
        }
    }
}
