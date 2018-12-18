
using UnityEngine;

// TODO: Do NOT use this class. It is unfinished.
namespace PJ {
	/// <summary>
	/// An angle with 2 components: angle (normalized) and magnitude
	/// (uses reading coordinates: 0 degrees is at the top.)
	/// </summary>
	class Angle2
	{
		public float magnitude = 1.0f;
		public float angle;	// 0-1.0 (.5 is 180 degrees always pointing screen down)

		public Angle2()
		{
		}
		// VERIFIED ABOVE:

		public Angle2(Vector2 vector)
		{
			float squaredMagnitude = vector.x*vector.x + vector.y*vector.y;
			magnitude = Mathf.Sqrt(squaredMagnitude);
			
			Vector2	vp = new Vector2(vector.x, vector.y);
			angle = AngleUtils.DistanceToDegTurnAngle(vp)/360.0f;	// Normalize it
		}

		public Vector2 ToMotionVector()
		{
			Vector2 result = new Vector2(0, 0);

			float _angle = angle * 360.0f;
			_angle = AngleUtils.ClipDegTurnAngle(_angle);
	
			Vector2 distance = AngleUtils.DegTurnAngleHypToDistance(_angle, magnitude);	// TODO: cartesian
			result = distance;
			
			return result;
		}

		public Vector2 GetScaledValue(float scale)
		{
			Vector2	pt = AngleUtils.DegTurnAngleHypToDistance(360.0f*angle, magnitude);
			Vector2	result = pt;
			return result;
		}
	}
}
