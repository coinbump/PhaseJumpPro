using System.Collections;
using System.Collections.Generic;
using Godot;
using PJ;

/*
 * RATING: 5 stars
 * Simple types
 * CODE REVIEW: 4/14/22
 */
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
		Southwest,
		West
	}

	/// <summary>
	/// Axis limits for map direction movement
	/// </summary>
	public enum MapDirectionAxisLimit
	{
		FourWay,

		EightWay
	}

	/// <summary>
	/// A rectangle in 2D space
	/// </summary>
	public struct Rect2I
	{
		public Vector2I position;
		public Vector2I size;

		public Rect2I(Vector2I position, Vector2I size)
		{
			this.position = position;
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


namespace PJ
{
	public static class Extensions_MapDirection
	{
		/// <summary>
		/// Returns the opposite direction
		/// </summary>
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

		/// <summary>
		/// Returns offset in matrix space (top-left is 0, 0)
		/// </summary>
		public static Vector2I Offset(this MapDirection state)
		{
			switch (state)
			{
				case MapDirection.Northwest:
					return new Vector2I(-1, -1);
				case MapDirection.North:
					return new Vector2I(0, -1);
				case MapDirection.Northeast:
					return new Vector2I(1, -1);
				case MapDirection.East:
					return new Vector2I(1, 0);
				case MapDirection.Southeast:
					return new Vector2I(1, 1);
				case MapDirection.South:
					return new Vector2I(0, 1);
				case MapDirection.Southwest:
					return new Vector2I(-1, 1);
				case MapDirection.West:
					return new Vector2I(-1, 0);
			}

			return new Vector2I(0, 0);
		}
	}

	public static class MapUtils
	{
		/// <summary>
		/// Translate a degree angle to a map direction
		/// </summary>
		public static MapDirection AngleToMapDirection(Angle angle, MapDirectionAxisLimit axisLimit)
		{
			var degreeAngle = angle.Degrees;
			var axisLimitNumber = 4;
			switch (axisLimit)
			{
				case MapDirectionAxisLimit.EightWay:
					axisLimitNumber = 8;
					break;
			}

			var angleAxisLimiter = new AngleAxisLimiter2D(axisLimitNumber);
			var limitedAngle = angleAxisLimiter.LimitAngle(Angle.DegreesAngle(degreeAngle));

			var result = MapDirection.North;

			switch (Mathf.RoundToInt(limitedAngle.Degrees))
			{
				case 0:
				case 360:
					return MapDirection.North;
				case 45:
					return MapDirection.Northeast;
				case 90:
					return MapDirection.East;
				case 135:
					return MapDirection.Southeast;
				case 180:
					return MapDirection.South;
				case 225:
					return MapDirection.Southwest;
				case 270:
					return MapDirection.West;
				case 315:
					return MapDirection.Northwest;
			}

			return result;
		}
	}
}
