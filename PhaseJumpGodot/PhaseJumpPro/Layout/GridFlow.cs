using System;
using Godot;

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
    public partial class GridFlow : SomeLayout2D
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

        [Export]
        public int width = 2;

        [Export]
        public Vector2 spacing = Vector2.Zero;

        protected GridLogicModel LogicModel
        {
            get
            {
                return new GridLogicModel(width, GetChildCount());
            }
        }

        public int ColumnCount => LogicModel.ColumnCount;
        public int RowCount => LogicModel.RowCount;

        public int RowIndexAt(int index) => LogicModel.RowIndexAt(index);
        public int ColumnIndexAt(int index) => LogicModel.ColumnIndexAt(index);

        public override Vector3 Size()
        {
            return new Vector3(spacing.X * (ColumnCount - 1), spacing.Y * (RowCount - 1), 0);
        }

        public override Vector3 LayoutPositionAt(int index)
        {
            var firstPos = new Vector2(-Size().X / 2, Size().Y / 2);

            var columnIndex = ColumnIndexAt(index);
            var rowIndex = RowIndexAt(index);

            // Optional: Attach a Location component to override standard grid flow
            if (index < GetChildCount())
            {
                var childObject = GetChild(index);

                // TODO: this won't work, because we can't attach a location as a component
                // if (childObject.TryGetComponent(out Location location))
                // {
                //     columnIndex = Math.Min(ColumnCount - 1, Math.Max(0, (int)location.location.X));
                //     rowIndex = (int)location.location.Y;
                // }
            }

            return new Vector3(firstPos.X + spacing.X * columnIndex, firstPos.Y - spacing.Y * rowIndex, 0);
        }

        public override void ApplyLayout()
        {
            int index = 0;

            foreach (var child in GetChildren())
            {
                var position = LayoutPositionAt(index);

                var transformTool = new TransformTool(child);
                transformTool.SetLocalPosition(position);

                index += 1;
            }
        }
    }
}
