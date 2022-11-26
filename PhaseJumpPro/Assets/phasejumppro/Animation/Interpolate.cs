using System;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple transform operators
 * CODE REVIEW: 4/13/22
 */
namespace PJ
{
	// For editors to pick from list
	public enum InterpolateType
	{
		Linear, Squared, Cubed, OutSquared, OutCubed
	}

	/// <summary>
	/// Interpolate a normalized float.(0-1.0)
	/// Like Unity's AnimationCurve. Platform neutral for future language porting
	/// </summary>
	public class Interpolate : SomeTransform<float>
	{
		public override float Transform(float value)
		{
			return value;
		}

		public static Interpolate linear = new InterpolateLinear();
	}

	public class InterpolateLinear : Interpolate
	{
		public override float Transform(float time)
		{
			return time;
		}
	}

    public class InterpolateSquared : Interpolate
	{
		public override float Transform(float time)
		{
			return time * time;
		}
	}

	public class InterpolateCubed : Interpolate
	{
		public override float Transform(float time)
		{
			return time * time * time;
		}
	}

	public class InterpolateOutSquared : Interpolate
	{
		public override float Transform(float time)
		{
			time = 1.0f - time;
			var result = 1.0f - (time * time);
			return result;
		}
	}

	public class InterpolateOutCubed : Interpolate
	{
		public override float Transform(float time)
		{
			time = 1.0f - time;
			var result = 1.0f - (time * time * time);
			return result;
		}
	}
}
