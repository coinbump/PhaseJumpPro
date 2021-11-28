using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

namespace PJ
{
	/// <summary>
	/// A tile that can be placed in a grid (EXAMPLE: diamond block in Bejeweled)
	/// </summary>
	public class GridTile : Core
	{
		public Vector3Int origin;	// Z is used for multi-layer grids
		public Vector2Int size;

		public WeakReference<SomeGridBoard> board;	// Owner. null if not in a board.
	}
}
