using System;
using Godot;

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
		public static float DistanceSquared(Vector2 p1, Vector2 p2) { return ((Mathf.Abs(p1.X - p2.X) * Mathf.Abs(p1.X - p2.X)) + (Mathf.Abs(p1.Y - p2.Y) * Mathf.Abs(p1.Y - p2.Y))); }
		public static float Distance(Vector2 p1, Vector2 p2) { return Mathf.Sqrt(DistanceSquared(p1, p2)); }

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

		public static float AngleOppToHyp(Angle angle, float opp)
		{
			float degreeAngle = angle.Degrees;
			float sinVal = Mathf.Sin(degreeAngle * Mathf.Deg2Rad);
			float hyp = opp * (1.0f / sinVal);
			return hyp;
		}

		/// <summary>
		/// Return index of corner based on angle (starting with top-left) 
		/// </summary>
		public static Optional<int> RectCornerAngleIndex(Angle angle)
		{
			var degreeAngle = angle.Clipped().Degrees;

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
		public static Angle NextRectCornerDegreeAngle(Angle degreeAngle)
		{
			var result = degreeAngle.Clipped();

			if (result.Degrees < 45.0f)
			{
				return Angle.DegreesAngle(45.0f);
			}
			if (result.Degrees < 135.0f)
			{
				return Angle.DegreesAngle(135.0f);
			}
			if (result.Degrees < 225.0f)
			{
				return Angle.DegreesAngle(225.0f);
			}
			if (result.Degrees < 315.0f)
			{
				return Angle.DegreesAngle(315.0f);
			}

			return degreeAngle;
		}

		/// <summary>
		/// Returns true if the angle points to a corner in a rectangle
		/// </summary>
		public static bool IsRectCornerAngle(Angle angle)
		{
			var degreeAngle = angle.Degrees;
			return Utils.Equals(45.0f, degreeAngle) || Utils.Equals(135.0f, degreeAngle) || Utils.Equals(225.0f, degreeAngle) || Utils.Equals(315.0f, degreeAngle);
		}
	}
}
