using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

namespace PJ
{
	/// <summary>
	/// 8-way direction of movement on a board/map
	/// </summary>
	public enum MapDirection
	{
		Northwest,
		North,
		Northeast,
		East,
		South,
		Southeast,
		Southwest
	}

	/// <summary>
	/// A rectangle in 2D space
	/// </summary>
	public struct Rect2Int
	{
		public Vector2Int origin;
		public Vector2Int size;

		public Rect2Int(Vector2Int origin, Vector2Int size)
		{
			this.origin = origin;
			this.size = size;
		}
	}

	/// <summary>
	/// Axial direction of travel
	/// </summary>
	public enum AxialDirection
	{
		Right,
		Left
	}

	public enum AxialType
	{
		AxialAny,   // Any axial tile that touches the origin tile
		AxialEdge   // Any axial tile that has an edge touching the origin tile (no square diagonal)
	}
}


namespace EnumExtension
{
	public static class HexBoardDirectionExtensions
	{
		public static MapDirection Opposite(this MapDirection direction)
		{
			switch (direction)
			{
				case MapDirection.Northwest:
					return MapDirection.Southeast;
				case MapDirection.North:
					return MapDirection.South;
				case MapDirection.Northeast:
					return MapDirection.Southwest;
				case MapDirection.Southeast:
					return MapDirection.Northwest;
				case MapDirection.South:
					return MapDirection.North;
				case MapDirection.Southwest:
					return MapDirection.Northeast;
			}

			return MapDirection.North;
		}
	}
}