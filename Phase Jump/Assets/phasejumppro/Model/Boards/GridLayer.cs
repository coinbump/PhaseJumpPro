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
		// FUTURE: add support for cell sets, fast distribition (for populating grid worlds).
	}
}
