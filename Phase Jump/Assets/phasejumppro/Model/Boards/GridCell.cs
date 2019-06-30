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
