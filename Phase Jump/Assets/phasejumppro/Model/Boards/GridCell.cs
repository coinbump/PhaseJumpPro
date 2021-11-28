using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars. Simple storage class
 * CODE REVIEW: 6.30.19
 */
namespace PJ
{
	/// <summary>
	/// Tile placement information in a grid (1 large tile might extend across multiple spaces)
	///
	/// Grids are 2-dimensional. Use z axis for layers
	/// </summary>
	public class GridCell<Tile> where Tile : GridTile
	{
		public Tile tile;
		public Vector3Int origin;

		public GridCell()
		{
		}

		public GridCell(Vector3Int origin)
		{
			this.origin = origin;
		}
	}
}
