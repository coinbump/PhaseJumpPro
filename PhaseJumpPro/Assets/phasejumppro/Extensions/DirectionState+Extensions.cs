using UnityEngine;
using System.Collections;

/*
 * RATING: 4 stars
 * Simple utility, could use unit tests
 * CODE REVIEW: 4/21/22
 */
namespace PJ
{
	/// <summary>
	/// Extensions for DirectionState type
	/// </summary>
	public static class Extensions_DirectionState
	{
		public static string ToString(this DirectionState state)
		{
			string result = "";

			switch (state)
			{
				case DirectionState.Up:
					result = "up";
					break;
				case DirectionState.UpRight:
					result = "up_right";
					break;
				case DirectionState.Right:
					result = "right";
					break;
				case DirectionState.DownRight:
					result = "down_right";
					break;
				case DirectionState.Down:
					result = "down";
					break;
				case DirectionState.DownLeft:
					result = "down_left";
					break;
				case DirectionState.Left:
					result = "left";
					break;
				case DirectionState.UpLeft:
					result = "up_left";
					break;
			}
			return result;
		}

		public static float ToDegreeAngle(this DirectionState state)
		{
			float result = 0.0f;

			switch (state)
			{
				case DirectionState.Up:
					result = 0.0f;
					break;
				case DirectionState.UpRight:
					result = 45.0f;
					break;
				case DirectionState.Right:
					result = 90.0f;
					break;
				case DirectionState.DownRight:
					result = 135.0f;
					break;
				case DirectionState.Down:
					result = 180.0f;
					break;
				case DirectionState.DownLeft:
					result = 225.0f;
					break;
				case DirectionState.Left:
					result = 270.0f;
					break;
				case DirectionState.UpLeft:
					result = 315.0f;
					break;
			}

			return result;
		}

		public static Vector3Int OffsetGridLoc(this DirectionState state, Vector3Int gridPos)
		{
			Vector3Int result = gridPos;

			switch (state)
			{
				case DirectionState.Left:
					result.x += (int)Vector2.left.x;
					break;
				case DirectionState.Right:
					result.x += (int)Vector2.right.x;
					break;
				case DirectionState.Up:
					result.y += (int)Vector2.up.y;
					break;
				case DirectionState.Down:
					result.y += (int)Vector2.down.y;
					break;
				case DirectionState.UpLeft:
					result.x += (int)Vector2.left.x;
					result.y += (int)Vector2.up.y;
					break;
				case DirectionState.UpRight:
					result.x += (int)Vector2.right.x;
					result.y += (int)Vector2.up.y;
					break;
				case DirectionState.DownLeft:
					result.x += (int)Vector2.left.x;
					result.y += (int)Vector2.down.y;
					break;
				case DirectionState.DownRight:
					result.x += (int)Vector2.right.x;
					result.y += (int)Vector2.down.y;
					break;
			}

			return result;
		}

		public static DirectionState Flip(this DirectionState state)
		{
			DirectionState result = state;

			switch (state)
			{
				case DirectionState.Left:
					result = DirectionState.Right;
					break;
				case DirectionState.Right:
					result = DirectionState.Left;
					break;
				case DirectionState.Up:
					result = DirectionState.Down;
					break;
				case DirectionState.Down:
					result = DirectionState.Up;
					break;
				case DirectionState.UpLeft:
					result = DirectionState.DownRight;
					break;
				case DirectionState.UpRight:
					result = DirectionState.DownLeft;
					break;
				case DirectionState.DownLeft:
					result = DirectionState.UpRight;
					break;
				case DirectionState.DownRight:
					result = DirectionState.UpLeft;
					break;
			}

			return result;
		}

		public static DirectionState RotateLeft(this DirectionState state, DirectionScheme scheme)
		{
			DirectionState result = state;

			switch (scheme)
			{
				case DirectionScheme.FourWay:
					switch (state)
					{
						case DirectionState.Left:
							result = DirectionState.Down;
							break;
						case DirectionState.Right:
							result = DirectionState.Up;
							break;
						case DirectionState.Up:
							result = DirectionState.Left;
							break;
						case DirectionState.Down:
							result = DirectionState.Right;
							break;
						default:
							break;
					}
					break;
				case DirectionScheme.EightWay:
					switch (state)
					{
						case DirectionState.Left:
							result = DirectionState.DownLeft;
							break;
						case DirectionState.DownLeft:
							result = DirectionState.Down;
							break;
						case DirectionState.Down:
							result = DirectionState.DownRight;
							break;
						case DirectionState.DownRight:
							result = DirectionState.Right;
							break;
						case DirectionState.Right:
							result = DirectionState.UpRight;
							break;
						case DirectionState.UpRight:
							result = DirectionState.Up;
							break;
						case DirectionState.Up:
							result = DirectionState.UpLeft;
							break;
						case DirectionState.UpLeft:
							result = DirectionState.Left;
							break;
					}
					break;
			}

			return result;
		}

		public static DirectionState RotateRight(this DirectionState state, DirectionScheme scheme)
		{
			DirectionState result = state;

			switch (scheme)
			{
				case DirectionScheme.FourWay:
					switch (state)
					{
						case DirectionState.Left:
							result = DirectionState.Up;
							break;
						case DirectionState.Right:
							result = DirectionState.Down;
							break;
						case DirectionState.Up:
							result = DirectionState.Right;
							break;
						case DirectionState.Down:
							result = DirectionState.Left;
							break;
					}
					break;
				case DirectionScheme.EightWay:
					switch (state)
					{
						case DirectionState.Left:
							result = DirectionState.UpLeft;
							break;
						case DirectionState.DownLeft:
							result = DirectionState.Left;
							break;
						case DirectionState.Down:
							result = DirectionState.DownLeft;
							break;
						case DirectionState.DownRight:
							result = DirectionState.Down;
							break;
						case DirectionState.Right:
							result = DirectionState.DownRight;
							break;
						case DirectionState.UpRight:
							result = DirectionState.Right;
							break;
						case DirectionState.Up:
							result = DirectionState.UpRight;
							break;
						case DirectionState.UpLeft:
							result = DirectionState.Up;
							break;
					}
					break;
			}
			return result;
		}
	}
}