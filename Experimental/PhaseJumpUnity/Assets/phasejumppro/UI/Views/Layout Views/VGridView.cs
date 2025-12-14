using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

/*
RATING: 4 stars
Has unit tests. Missing advanced layout (requires constraints solver)
CODE REVIEW: 6/11/23
*/
namespace PJ
{
    /// <summary>
    /// Arranges views in a grid that grows vertically
    /// </summary>
    public partial class VGridView : View2D
    {
        [Header("VGridView Properties")]
        /// (EDITOR ONLY: values are moved on Awake)
        public List<GridViewItemModel> _columns = new();

        public float rowSpacing = 0f;

        /// <summary>
        /// Alignment for the grid in layout view space
        /// </summary>
        public HorizontalAlignment alignment = HorizontalAlignment.center;

        public List<GridViewItem> Columns => columns;

        protected List<GridViewItem> columns = new();

        public override void UpdateFromSerializedProperties(bool forceUpdate)
        {
            base.UpdateFromSerializedProperties(forceUpdate);

            if (columns.Count == 0 || forceUpdate)
            {
                columns = _columns.ConvertAll(new Converter<GridViewItemModel, GridViewItem>(GridViewItemModelToGridViewItem));
            }
        }

        public static GridViewItem GridViewItemModelToGridViewItem(GridViewItemModel model)
        {
            HorizontalAlignment horizontalAlignment = new HorizontalAlignment.Builder().HorizontalAlignmentFrom(model.horizontalAlignment);
            VerticalAlignment verticalAlignment = new VerticalAlignment.Builder().VerticalAlignmentFrom(model.verticalAlignment);
            var alignment = new Alignment(horizontalAlignment, verticalAlignment);

            return new(
                model.sizeType,
                model.size,
                alignment,
                model.spacing
            );
        }

        protected float GridWidth(Vector2 layoutSize)
        {
            var fixedWidthColumns = columns.Where((column) => column.sizeType == GridViewItem.SizeType.Fixed).ToList();
            var isAllFixedWidth = fixedWidthColumns.Count == columns.Count;

            float gridSize = 0;

            // FUTURE: support alignment for flexible min-max (requires constraints solver)
            if (isAllFixedWidth)
            {
                for (int i = 0; i < fixedWidthColumns.Count; i++)
                {
                    var column = fixedWidthColumns[i];
                    gridSize += column.size;

                    if (i < fixedWidthColumns.Count - 1)
                    {
                        gridSize += column.spacing;
                    }
                }
            }
            else
            {
                return layoutSize.x;
            }

            return gridSize;
        }

        protected override void _ApplyLayout(Vector2 layoutSize)
        {
            var layoutModel = BuildLayoutModel(layoutSize);

            var fixedWidthColumns = columns.Where((column) => column.sizeType == GridViewItem.SizeType.Fixed).ToList();
            var isAllFixedWidth = fixedWidthColumns.Count == columns.Count;
            float gridSize = GridWidth(layoutSize);

            float y = 0;
            foreach (var row in layoutModel.rows)
            {
                var rowHeight = row.size;

                for (int columnIndex = 0; columnIndex < row.views.Count && columnIndex < columns.Count; columnIndex++)
                {
                    var view = row.views[columnIndex];
                    var frame = new Rect();

                    var columnWidth = layoutSize.x;
                    float x = 0;

                    frame.origin.x = x;
                    frame.origin.y = y;

                    var layoutColumn = layoutModel.columns[columnIndex];
                    columnWidth = layoutColumn.size;
                    frame.origin.x = layoutColumn.position;

                    var viewPreferredWidth = view.PreferredWidthWithConstraints(new(columnWidth, layoutSize.y));
                    frame.size.x = viewPreferredWidth != null ? viewPreferredWidth.value : columnWidth;
                    var viewPreferredHeight = view.PreferredHeightWithConstraints(new Vector2(frame.size.x, rowHeight));
                    frame.size.y = viewPreferredHeight != null ? viewPreferredHeight.value : rowHeight;

                    var alignedOriginX = layoutColumn.alignment.horizontalAlignment.aligner.AlignedOrigin(columnWidth, frame.size.x);
                    frame.origin.x += alignedOriginX;

                    var alignedOriginY = layoutColumn.alignment.verticalAlignment.aligner.AlignedOrigin(rowHeight, frame.size.y);
                    frame.origin.y += alignedOriginY;

                    if (gridSize > 0)
                    {
                        var sizedAlignedOriginX = alignment.aligner.AlignedOrigin(layoutSize.x, gridSize);
                        frame.origin.x += sizedAlignedOriginX;
                    }

                    view.Frame = frame;
                }

                y += (rowHeight + rowSpacing);
            }
        }
    }
}
