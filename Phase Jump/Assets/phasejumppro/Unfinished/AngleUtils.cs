using System;
using UnityEngine;

namespace PJ {
	// TODO: Do NOT use this class. It is unfinished.
	public class AngleUtils {
		public static Vector2 DegreeAngleToVector2(float degreeAngle)
		{
			var radians = degreeAngle * Mathf.Deg2Rad;
			Vector2 result = new Vector2(Mathf.Sin(radians), Mathf.Cos(radians)*Vector2.up.y);
			return result;
		}
	
		public static float DistanceSquared(Vector2 p1, Vector2 p2) { return ((Mathf.Abs(p1.x - p2.x) * Mathf.Abs(p1.x - p2.x)) + (Mathf.Abs(p1.y - p2.y) * Mathf.Abs(p1.y - p2.y))); }
		public static float Distance(Vector2 p1, Vector2 p2) { return Mathf.Sqrt(DistanceSquared(p1, p2)); }

		/*
			GetSquaredHypotenuse

			Converts x, y distance into squared angle distance (always positive).

		 */
		public static float GetSquaredHypotenuse(float distanceX, float distanceY)
		{
			Vector2 point1 = new Vector2(0, 0);
			Vector2 point2 = new Vector2(distanceX, distanceY);

			float result = DistanceSquared(point1, point2);
			return result;

		}

		/// <summary>
		/// Gets the hypotenuse.
		/// </summary>
		/// <returns>The hypotenuse.</returns>
		/// <param name="distanceX">Distance x.</param>
		/// <param name="distanceY">Distance y.</param>
		public static float GetHypotenuse(float distanceX, float distanceY)
		{
			Vector2 point1 = new Vector2(0, 0);
			Vector2 point2 = new Vector2(distanceX, distanceY);

			float result = Distance(point1, point2);
			return result;
		}

		/*
			DegTurnAngleHypToDistance

			IMPORTANT: this is the degree angle in angular degrees from rotation zero, *not*
			the adjacent-opposite of a right triangle with this angle.

		 */
		public static Vector2 DegTurnAngleHypToDistance(float degAngle, float distance)
		{
			Vector2 result = new Vector2(0, 0);

			float sinVal = Mathf.Sin(degAngle* Mathf.Deg2Rad);
			float cosVal = Mathf.Cos(degAngle * Mathf.Deg2Rad);

			float x = distance * sinVal;
			float y = -(distance * cosVal);
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
		public static float GetClosestDegTurn(float firstAngle, float finalAngle)
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

		public static float ClipDegTurnAngle(float angle)
		{
			angle = angle % 360.0f;
			if (angle < 0)
			{
				angle += 360.0f;
			}
			return angle;
		}

		/*
			DistanceToDegTurnAngle

			Returns the angle of the x, y distance travelled (0 to 359).

			IMPORTANT: these computations are in READING coordinates, NOT Cartesian coordinates.

		 */
		public static float DistanceToDegTurnAngle(Vector2 distance)
		{
			if (distance.x == 0 && distance.y == 0)
			{
				return 0;
			}

			float result = ClipDegTurnAngle((Mathf.Rad2Deg * Mathf.Atan2(distance.y, distance.x)) + 90);
			return result;
		}

		public static bool IsRectCornerDegAngle(float degAngle)
		{
			return 45.0f == degAngle || 135.0f == degAngle || 225.0f == degAngle || 315.0f == degAngle;

		}

		// TODO: use epsilons.
		public static int GetRectCornerDegAngleIndex(float degAngle)
		{
			degAngle = degAngle % 360.0f;

			if (45.0f == degAngle)
			{
				return 1;
			}
			if (135.0f == degAngle)
			{
				return 2;
			}
			if (225.0f == degAngle)
			{
				return 3;
			}
			if (315.0f == degAngle)
			{
				return 0;
			}

			return -1;

		}

		public static float GetNextRectCornerDegAngle(float degAngle)
		{
			float result = degAngle % 360.0f;

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
	}
}
