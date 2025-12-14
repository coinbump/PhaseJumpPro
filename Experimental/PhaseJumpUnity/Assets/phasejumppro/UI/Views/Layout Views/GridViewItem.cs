using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Values model
CODE REVIEW: 6/10/23
*/
namespace PJ
{
    public struct GridViewItem
    {
        public enum SizeType
        {
            // Fixed size column/row, based on .size
            Fixed = 0,

            // FUTURE: To support flexible with min-max, we need a constraints solver
            // Fits available width
            Flexible = 1
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

    /// <summary>
    /// For serializing GridViewItem
    /// </summary>
    [Serializable]
    public struct GridViewItemModel
    {
        public GridViewItem.SizeType sizeType;
        public float size;
        public HorizontalAlignmentType horizontalAlignment;
        public VerticalAlignmentType verticalAlignment;
        public float spacing;
    }
}
