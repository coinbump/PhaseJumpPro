using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/21/22
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

		/// <summary>
        /// Clip the degree angle to stay between (0-360)
        /// Example: 361 becomes 1
        /// </summary>
        public static float ClipDegreeAngle(float angle)
		{
			angle %= 360.0f;
			if (angle < 0)
			{
				angle = 360.0f - (Mathf.Abs(angle) % 360.0f);
			}
			return angle;
		}

		/// <summary>
		/// Convert x, y distance into squared angle distance
		/// </summary>
		public static float SquaredHypotenuse(float distanceX, float distanceY)
		{
			Vector2 v1 = new Vector2(0, 0);
			Vector2 v2 = new Vector2(distanceX, distanceY);

			float result = DistanceSquared(v1, v2);
			return result;
		}

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

		public static Vector2 DegreeAngleToVector2(float degreeAngle, float distance)
		{
			Vector2 result = new Vector2(0, 0);

			float sinVal = Mathf.Sin(degreeAngle * Mathf.Deg2Rad);
			float cosVal = Mathf.Cos(degreeAngle * Mathf.Deg2Rad);

			float x = distance * sinVal;
			float y = distance * cosVal * Vector2.up.y;
			result.x = x;
			result.y = y;

			return result;
		}

		public static float DegreeAngleOppToHyp(float degreeAngle, float opp)
		{
			float sinVal = Mathf.Sin(degreeAngle * Mathf.Deg2Rad);
			float hyp = opp * (1.0f / sinVal);
			return hyp;
		}

		/// <summary>
		/// Returns the closest turn between two angles.
		/// EXAMPLE: closest turn between angle 3 and angle 359 is -4, not 356.
		/// </summary>
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

		public static float Vector2ToDegreeAngle(Vector2 distance)
		{
			if (Utils.Equals(distance.x, 0) && Utils.Equals(distance.y, 0))
			{
				return 0;
			}

			float radians = Mathf.Atan2(distance.y * Vector2.down.y, distance.x);
			float angle = (Mathf.Rad2Deg * radians) + 90;
			float result = ClipDegreeAngle(angle);
			return result;
		}

		/// <summary>
		/// Return index of corner based on angle (starting with top-left) 
		/// </summary>
		public static Optional<int> RectCornerDegreeAngleIndex(float degreeAngle)
		{
			degreeAngle = ClipDegreeAngle(degreeAngle);

			if (Utils.Equals(45.0f, degreeAngle))
			{
				return new Optional<int>(1);
			}
			if (Utils.Equals(135.0f, degreeAngle))
			{
				return new Optional<int>(2);
			}
			if (Utils.Equals(225.0f, degreeAngle))
			{
				return new Optional<int>(3);
			}
			if (Utils.Equals(315.0f, degreeAngle))
			{
				return new Optional<int>(0);
			}

			return null;
		}

		/// <summary>
		/// Return the angle of the next corner to turn to in a rect (clockwise)
		/// </summary>
		public static float NextRectCornerDegreeAngle(float degreeAngle)
		{
			float result = ClipDegreeAngle(degreeAngle);

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

			return degreeAngle;
		}

		/// <summary>
		/// Returns true if the angle points to a corner in a rectangle
		/// </summary>
		public static bool IsRectCornerDegreeAngle(float degreeAngle)
		{
			return Utils.Equals(45.0f, degreeAngle) || Utils.Equals(135.0f, degreeAngle) || Utils.Equals(225.0f, degreeAngle) || Utils.Equals(315.0f, degreeAngle);
		}
	}
}
