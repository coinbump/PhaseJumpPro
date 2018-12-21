using System;
using UnityEngine;

/*
 * RATING: 5 stars. Simple utilties with unit tests
 * CODE REVIEW: 12/21/18
 */
namespace PJ
{
	/// <summary>
	/// Utilities for dealing with angles in 2D space.
	/// </summary>
	public static class AngleUtils
	{

		public static float DistanceSquared(Vector2 p1, Vector2 p2) { return ((Mathf.Abs(p1.x - p2.x) * Mathf.Abs(p1.x - p2.x)) + (Mathf.Abs(p1.y - p2.y) * Mathf.Abs(p1.y - p2.y))); }
		public static float Distance(Vector2 p1, Vector2 p2) { return Mathf.Sqrt(DistanceSquared(p1, p2)); }

		public static float ClipDegreeAngle(float angle)
		{
			angle %= 360.0f;
			if (angle < 0)
			{
				angle += 360.0f;
				angle %= 360.0f;    // Reduces 360.0f to 0
			}
			return angle;
		}

		/*
			SquaredHypotenuse

			Converts x, y distance into squared angle distance (always positive).

		 */
		public static float SquaredHypotenuse(float distanceX, float distanceY)
		{
			Vector2 point1 = new Vector2(0, 0);
			Vector2 point2 = new Vector2(distanceX, distanceY);

			float result = DistanceSquared(point1, point2);
			return result;
		}

		/// <summary>
		/// GetGetSquaredHypotenuse/// </summary>
		/// <returns>The hypotenuse.</returns>
		/// <param name="distanceX">Distance x.</param>
		/// <param name="distanceY">Distance y.</param>
		public static float Hypotenuse(float distanceX, float distanceY)
		{
			Vector2 point1 = new Vector2(0, 0);
			Vector2 point2 = new Vector2(distanceX, distanceY);

			float result = Distance(point1, point2);
			return result;
		}

		public static Vector2 Angle2DToVector2(Angle2D angle)
		{
			return DegreeAngleToVector2(angle.angle * 360.0f, angle.magnitude);
		}

		/*
			DegreeAngleToVector2

			IMPORTANT: this is the degree angle in angular degrees from rotation zero, *not*
			the adjacent-opposite of a right triangle with this angle.

		 */
		public static Vector2 DegreeAngleToVector2(float degAngle, float distance)
		{
			Vector2 result = new Vector2(0, 0);

			float sinVal = Mathf.Sin(degAngle * Mathf.Deg2Rad);
			float cosVal = Mathf.Cos(degAngle * Mathf.Deg2Rad);

			float x = distance * sinVal;
			float y = distance * cosVal * Vector2.up.y;
			result.x = x;
			result.y = y;

			return result;
		}

		public static float DegAngleOppToHyp(float degAngle, float opp)
		{
			float sinVal = Mathf.Sin(degAngle * Mathf.Deg2Rad);
			float hyp = opp * (1.0f / sinVal);
			return hyp;
		}

		/// <summary>
		/// Returns the closest turn between two angles.
		/// EXAMPLE: closest turn between angle 3 and angle 359 is -4, not 356.
		/// </summary>
		/// <returns>The closest deg turn.</returns>
		/// <param name="firstAngle">First angle.</param>
		/// <param name="finalAngle">Final angle.</param>
		public static float GetClosestDegreeTurn(float firstAngle, float finalAngle)
		{
			float delta1, delta2;
			float result = 0;

			if (finalAngle > firstAngle)
			{
				delta1 = finalAngle - firstAngle;
				delta2 = -(360 - delta1);
			}
			else
			{
				delta1 = -(firstAngle - finalAngle);
				delta2 = 360 + delta1;
			}

			if (Mathf.Abs(delta1) < Mathf.Abs(delta2))
			{
				result = delta1;
			}
			else
			{
				result = delta2;
			}

			return result;
		}

		/*
			Vector2ToDegreeAngle

			Returns the angle of the x, y distance travelled (0 to 359).
		 */
		public static float Vector2ToDegreeAngle(Vector2 distance)
		{
			if (Utils.CompareEqual(distance.x, 0) && Utils.CompareEqual(distance.y, 0))
			{
				return 0;
			}

			float radians = Mathf.Atan2(distance.y * Vector2.down.y, distance.x);
			float angle = (Mathf.Rad2Deg * radians) + 90;
			float result = ClipDegreeAngle(angle);
			return result;
		}

		// Return index of corner based on angle (starting with top-left)
		public static int GetRectCornerDegreeAngleIndex(float degAngle)
		{
			degAngle = ClipDegreeAngle(degAngle);

			if (Utils.CompareEqual(45.0f, degAngle))
			{
				return 1;
			}
			if (Utils.CompareEqual(135.0f, degAngle))
			{
				return 2;
			}
			if (Utils.CompareEqual(225.0f, degAngle))
			{
				return 3;
			}
			if (Utils.CompareEqual(315.0f, degAngle))
			{
				return 0;
			}

			return -1;
		}

		// Return the angle of the next corner to turn to in a rect (clockwise).
		public static float GetNextRectCornerDegAngle(float degAngle)
		{
			float result = ClipDegreeAngle(degAngle);

			if (result < 45.0f)
			{
				return 45.0f;
			}
			if (result < 135.0f)
			{
				return 135.0f;
			}
			if (result < 225.0f)
			{
				return 225.0f;
			}
			if (result < 315.0f)
			{
				return 315.0f;
			}

			return degAngle;
		}

		// Returns true if the angle points to a corner in a rectangle
		public static bool IsRectCornerDegreeAngle(float degAngle)
		{
			return Utils.CompareEqual(45.0f, degAngle) || Utils.CompareEqual(135.0f, degAngle) || Utils.CompareEqual(225.0f, degAngle) || Utils.CompareEqual(315.0f, degAngle);
		}
	}
}
