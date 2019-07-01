using UnityEngine;
using System.Collections;

namespace PJ
{
	using Tile = AbstractGridTile;

	/// <summary>
	/// Stores model data for each layer of the grid board
	/// </summary>
	class GridLayer : GenericGrid<GridCell>
	{
		// FUTURE: add support for cell sets, fast distribution (for populating grid worlds).

		public bool IsCellBlocked(Vector2Int loc) {
			if (!IsValidLoc(loc)) { return true; }
			GridCell cell = GetCell(loc);
			if (null == cell) {
				return false;	// Nothing there.
			}
			if (null != cell.tile && cell.tile.isGhost) {
				return false;	// Ghost tile.
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

	}
}
