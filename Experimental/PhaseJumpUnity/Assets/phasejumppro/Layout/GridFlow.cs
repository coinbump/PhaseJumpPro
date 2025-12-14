using System;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars
 * Standard flow, with unit tests
 * CODE REVIEW: 1/14/22
 */
namespace PJ
{
    /// <summary>
    /// Flow the objects with non-contextual spacing (object size doesn't matter)
    /// </summary>
    public class GridFlow : SomeLayout2D
    {
        /// <summary>
        /// Seperate logic model for unit tests
        /// </summary>
        public struct GridLogicModel
        {
            public int width;
            public int childCount;

            public GridLogicModel(int width, int childCount)
            {
                this.width = width;
                this.childCount = childCount;
            }

            public int ColumnCount => width;

            public int RowIndexAt(int index)
            {
                return index / ColumnCount;
            }

            public int ColumnIndexAt(int index)
            {
                return index % ColumnCount;
            }

            public int RowCount
            {
                get
                {
                    if (childCount == 0) { return 0; }

                    return (int)Math.Ceiling((float)childCount / (float)ColumnCount);
                }
            }
        }

        public int width = 2;
        public Vector2 spacing = Vector2.zero;

        protected GridLogicModel LogicModel
        {
            get
            {
                return new GridLogicModel(width, transform.childCount);
            }
        }

        public int ColumnCount => LogicModel.ColumnCount;
        public int RowCount => LogicModel.RowCount;

        public int RowIndexAt(int index) => LogicModel.RowIndexAt(index);
        public int ColumnIndexAt(int index) => LogicModel.ColumnIndexAt(index);

        public override Vector3 Size()
        {
            return new Vector3(spacing.x * (ColumnCount - 1), spacing.y * (RowCount - 1), 0);
        }

        public override Vector3 LayoutPositionAt(int index)
        {
            var firstPos = new Vector2(-Size().x / 2, Size().y / 2);

            var columnIndex = ColumnIndexAt(index);
            var rowIndex = RowIndexAt(index);

            // Optional: Attach a Location component to override standard grid flow
            if (index < transform.childCount)
            {
                var childObject = transform.GetChild(index).gameObject;
                if (childObject.TryGetComponent(out Location location))
                {
                    columnIndex = Math.Min(ColumnCount - 1, Math.Max(0, (int)location.location.x));
                    rowIndex = (int)location.location.y;
                }
            }

            return new Vector3(firstPos.x + spacing.x * columnIndex, firstPos.y - spacing.y * rowIndex, 0);
        }

        public override void ApplyLayout()
        {
            int index = 0;

            foreach (Transform childTransform in gameObject.transform)
            {
                var position = LayoutPositionAt(index);

                childTransform.localPosition = position;

                index += 1;
            }
        }
    }
}
