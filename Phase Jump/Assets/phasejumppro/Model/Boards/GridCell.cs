using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
	/// <summary>
	/// Tile placement information in a grid (1 large tile might extend across multiple spaces)
	/// </summary>
	public class GridCell
	{
		public AbstractGridTile tile;
		public Vector3Int origin;	// Z can be used or ignored

		public GridCell(Vector3Int origin)
		{
			this.origin = origin;
		}
	}
}
