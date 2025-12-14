using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple storage class
 * CODE REVIEW: 6.30.19
 */
namespace PJ
{
	/// <summary>
	/// Tile placement information in a grid (1 large tile might extend across multiple spaces)
	/// </summary>
	public class GridCell<Tile> where Tile : GridTile
	{
		public Tile tile;
		public Vector2Int position;

		public GridCell()
		{
		}

		public GridCell(Vector2Int position)
		{
			this.position = position;
		}
	}
}
