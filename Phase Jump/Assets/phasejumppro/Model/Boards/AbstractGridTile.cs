using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
	/// <summary>
	/// A tile that can be placed in a grid (EXAMPLE: diamond block in Bejeweled)
	/// </summary>
	public abstract class AbstractGridTile
	{
		public Vector3Int origin;   // Z can be used or ignored
		public Vector2Int size;

		// OPTIONAL attributes:
		public bool isPainted;  // True: was painted for render
		public bool isGhost;    // True: ignore for collision tests
	}
}
