using Godot;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 4/16/22
 */
namespace PJ
{
    using Tile = GridTile;

    /// <summary>
    /// Stores model data for each layer of the grid board
    /// </summary>
    public class GridLayer<Tile> : Matrix<GridCell<Tile>> where Tile : GridTile
    {
        public HashSet<Tile> tiles = new HashSet<Tile>();

        public bool IsCellBlocked(Vector2I loc)
        {
            if (!IsValidLocation(loc)) { return true; }
            GridCell<Tile> cell = CellAt(loc);
            if (null == cell)
            {
                return false;   // Nothing there.
            }
            return null != cell.tile;
        }

        public bool IsBlocked(Rect2Int bounds)
        {
            for (int x = bounds.position.X; x < (bounds.position.X + bounds.size.X); x++)
            {
                for (int y = bounds.position.Y; y < (bounds.position.Y + bounds.size.Y); y++)
                {
                    if (IsCellBlocked(new Vector2I(x, y)))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        public GridLayer(Vector2I size)
            : base(size)
        {
        }

        public virtual void OnCellsBlocked(Rect2Int bounds) { }
        public virtual void OnCellsUnblocked(Rect2Int bounds) { }
    }
}
