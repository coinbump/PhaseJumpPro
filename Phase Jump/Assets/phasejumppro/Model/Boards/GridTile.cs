using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
	/// <summary>
	/// A tile that can be placed in a grid (EXAMPLE: diamond block in Bejeweled)
	/// </summary>
	public class GridTile : Core
	{
		public Vector3Int origin;	// Z is used for multi-layer grids
		public Vector2Int size;

		public SomeGridBoard board;

		// OPTIONAL attributes:
		public bool isPainted;  // True: was painted for render
		public bool isGhost;    // True: ignore for collision tests
	}
}
