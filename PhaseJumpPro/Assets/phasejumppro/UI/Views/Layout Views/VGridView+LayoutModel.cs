using System;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

/*
RATING: 4 stars
Works. Has unit tests. Missing advanced layout (requires constraints solver)
CODE REVIEW: 6/17/23
*/
namespace PJ
{
    /// <summary>
    /// Arranges views in a grid that grows vertically
    /// </summary>
    public partial class VGridView
    {
        public struct LayoutColumn
        {
            public float position;
            public float size;
            public Alignment alignment;

            public LayoutColumn(float position, float size, Alignment alignment)
            {
                this.position = position;
                this.size = size;
                this.alignment = alignment;
            }
        }

        public struct LayoutRow
        {
            public List<View2D> views;

            /// <summary>
            /// Row height determined by size of cell content (might be null)
            /// </summary>
            public Optional<float> _size;

            /// <summary>
            /// Final row height
            /// </summary>
            public float size;

            public LayoutRow(List<View2D> views, Optional<float> _size)
            {
                this.views = views;
                this._size = _size;
                this.size = 0f;
            }
        }

        public struct LayoutModel
        {
            public List<LayoutColumn> columns;
            public List<LayoutRow> rows;

            public LayoutModel(List<LayoutColumn> columns, List<LayoutRow> rows)
            {
                this.columns = columns;
                this.rows = rows;
            }
        }

        public LayoutModel BuildLayoutModel(Vector2 layoutSize)
        {
            var columns = new List<GridViewItem>(this.columns);
            var childViews = ChildViews();

            var layoutColumns = new List<LayoutColumn>();
            float position = 0;

            var fixedWidthColumns = columns.Where((column) => column.sizeType == GridViewItem.SizeType.Fixed).ToList();
            var flexibleWidthColumns = columns.Where((column) => column.sizeType == GridViewItem.SizeType.Flexible).ToList();

            var spaceAvailable = layoutSize.x;

            for (int i = 0; i < (columns.Count - 1); i++)
            {
                spaceAvailable -= columns[i].spacing;
            }
            foreach (var column in fixedWidthColumns)
            {
                spaceAvailable -= column.size;
            }

            var flexibleWidth = 0f;
            if (flexibleWidthColumns.Count > 0)
            {
                flexibleWidth = spaceAvailable / flexibleWidthColumns.Count;
            }

            // Avoid infinite loop
            if (columns.Count == 0)
            {
                columns.Add(new(GridViewItem.SizeType.Flexible, 0, new()));
            }

            foreach (var column in columns)
            {
                var alignment = column.alignment != null ? column.alignment : new Alignment();

                switch (column.sizeType)
                {
                    // FUTURE: support Flexible width with a constraints solver
                    case GridViewItem.SizeType.Fixed:
                        {
                            var layoutColumn = new LayoutColumn(position, column.size, alignment);
                            layoutColumns.Add(layoutColumn);
                            position += (layoutColumn.size + column.spacing);
                            break;
                        }
                    case GridViewItem.SizeType.Flexible:
                        {
                            var layoutColumn = new LayoutColumn(position, flexibleWidth, alignment);
                            layoutColumns.Add(layoutColumn);
                            position += (layoutColumn.size + column.spacing);
                            break;
                        }
                }
            }

            var layoutRows = new List<LayoutRow>();
            var rowChildIndex = 0;

            while (rowChildIndex < childViews.Count)
            {
                var row = new LayoutRow(new(), null);

                for (int i = 0; i < columns.Count && rowChildIndex < childViews.Count; i++)
                {
                    var view = childViews[rowChildIndex];
                    rowChildIndex++;

                    var height = view.IntrinsicHeight;
                    if (null != height)
                    {
                        var rowHeight = null != row._size ? row._size.value : 0;
                        row._size = new(MathF.Max(rowHeight, height.value));
                    }

                    row.views.Add(view);
                }

                layoutRows.Add(row);
            }

            for (int i = 0; i < layoutRows.Count; i++)
            {
                var row = layoutRows[i];
                row.size = row._size != null ? row._size.value : (layoutSize.y - ((layoutRows.Count - 1) * rowSpacing)) / layoutRows.Count;
                layoutRows[i] = row;
            }

            return new LayoutModel(layoutColumns, layoutRows);
        }
    }
}
