using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

/*
RATING: 4 stars
Works. Has unit tests. Missing advanced layout (requires constraints solver)
CODE REVIEW: 11/30/22
*/
namespace PJ
{
    /// <summary>
    /// Arranges views in z-space stacked on top of each other
    /// </summary>
    public partial class VGridView : View2D
    {
        [Header("VGridView Properties")]
        public List<GridViewItem> columns = new();

        public float rowSpacing = 0f;

        /// <summary>
        /// Alignment for the grid in layout view space
        /// </summary>
        public HorizontalAlignment alignment = HorizontalAlignment.center;

        protected override void _ApplyLayout(Vector2 layoutSize)
        {
            var layoutModel = BuildLayoutModel(layoutSize);

            var fixedWidthColumns = columns.Where((column) => column.sizeType == GridViewItem.SizeType.Fixed).ToList();
            var isAllFixedWidth = fixedWidthColumns.Count == columns.Count;
            float gridSize = 0;

            // FUTURE: support alignment for flexible min-max (requires constraints solver)
            if (isAllFixedWidth)
            {
                foreach (var column in fixedWidthColumns)
                {
                    gridSize += column.size;
                }
            }

            float y = 0;
            foreach (var row in layoutModel.rows)
            {
                var rowHeight = row.size;

                for (int columnIndex = 0; columnIndex < row.views.Count; columnIndex++)
                {
                    var view = row.views[columnIndex];
                    var frame = new Bounds2D();

                    var columnWidth = layoutSize.x;
                    float x = 0;

                    frame.origin.x = x;
                    frame.origin.y = y;

                    if (columnIndex < columns.Count)
                    {
                        var layoutColumn = layoutModel.columns[columnIndex];
                        columnWidth = layoutColumn.size;
                        frame.origin.x = layoutColumn.position;

                        frame.size.x = view.PreferredWidth(columnWidth);
                        frame.size.y = view.PreferredHeight(new Vector2(frame.size.x, rowHeight));

                        var alignedOriginX = layoutColumn.alignment.horizontalAlignment.aligner.AlignedOrigin(columnWidth, frame.size.x);
                        frame.origin.x += alignedOriginX;

                        var alignedOriginY = layoutColumn.alignment.verticalAlignment.aligner.AlignedOrigin(rowHeight, frame.size.y);
                        frame.origin.y += alignedOriginY;
                    }
                    else
                    {
                        frame.size.x = view.PreferredWidth(layoutSize.x);
                        frame.size.y = view.PreferredHeight(new Vector2(frame.size.x, rowHeight));
                    }

                    if (gridSize > 0)
                    {
                        var alignedOriginX = alignment.aligner.AlignedOrigin(layoutSize.x, gridSize);
                        frame.origin.x += alignedOriginX;
                    }

                    view.Frame = frame;
                }

                y += (rowHeight + rowSpacing);
            }
        }
    }
}
