using UnityEngine;
using System.Collections;

namespace PJ
{
	using Tile = GridTile;

	/// <summary>
	/// Stores model data for each layer of the grid board
	/// </summary>
	public class GridLayer<Tile> : Grid<GridCell<Tile>> where Tile : GridTile
	{
		// FUTURE: add support for cell sets, fast distribution (for populating grid worlds).

		public bool IsCellBlocked(Vector2Int loc) {
			if (!IsValidLoc(loc)) { return true; }
			GridCell<Tile> cell = GetCell(loc);
			if (null == cell) {
				return false;	// Nothing there.
			}
			return null != cell.tile;
		}

		public bool IsBlocked(Rect2Int bounds) {
			for (int x = bounds.origin.x; x < (bounds.origin.x + bounds.size.x); x++) {
				for (int y = bounds.origin.y; y < (bounds.origin.y + bounds.size.y); y++) {
					if (IsCellBlocked(new Vector2Int(x, y))) {
						return true;
					}
				}
			}
	
			return false;
		}

		public GridLayer(Vector2Int size)
			: base(size)
		{
		}

		#region Events
		public virtual void EvtCellsBlocked(Rect2Int blocked) { }
		public virtual void EvtCellsUnblocked(Rect2Int blocked) { }
		#endregion
	}
}
