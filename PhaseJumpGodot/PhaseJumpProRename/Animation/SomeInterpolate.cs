using System;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 11/26/22
 * PORTED TO: C++
 */
namespace PJ
{
	/// <summary>
    /// For editors to pick from list of types
    /// </summary>
	public enum InterpolateType
	{
		Linear,

		InSquared,
		InCubed,
		OutSquared,
		OutCubed,
		OutElastic,
		InBack,
		OutBack,
		InOutBack
	}

	/// <summary>
	/// Interpolate a normalized float.(0-1.0)
	/// Like Unity's AnimationCurve. Platform neutral for future language porting
	/// </summary>
	public abstract class SomeInterpolate : SomeValueTransform<float>
	{
	}

	public class LinearInterpolate : SomeInterpolate
	{
		public override float Transform(float time)
		{
			return time;
		}
	}

    public class EaseInSquared : SomeInterpolate
	{
		public override float Transform(float time)
		{
            return MathF.Pow(time, 2);
		}
	}

	public class EaseInCubed : SomeInterpolate
	{
		public override float Transform(float time)
		{
            return MathF.Pow(time, 3);
        }
	}

	public class EaseOutSquared : SomeInterpolate
	{
		public override float Transform(float time)
		{
			return 1.0f - MathF.Pow(1.0f - time, 2);
		}
	}

	public class EaseOutCubed : SomeInterpolate
	{
		public override float Transform(float time)
		{
			return  1.0f - MathF.Pow(1.0f - time, 3);
		}
	}

	public class EaseOutElastic : SomeInterpolate 
	{
		public override float Transform(float time)
		{
            float x = time;
            float c4 = (2.0f * MathF.PI) / 3.0f;
			return x == 0 ? 0 : x == 1 ? 1 : MathF.Pow(2, -15 * x) * MathF.Sin((x * 10.0f - 0.75f) * c4) + 1.0f;
		}
	}

	public class EaseInBack : SomeInterpolate 
	{
		public override float Transform(float time)
		{
            float x = time;
            float c1 = 1.70158f;
			float c3 = c1 + 1;
			return c3 * x * x * x - c1 * x * x;
		}
	}

	public class EaseOutBack : SomeInterpolate 
	{
		public override float Transform(float time)
		{
            float x = time;
            float c1 = 1.70158f;
			float c3 = c1 + 1;
			return 1 + c3 * MathF.Pow(x - 1, 3) + c1 * MathF.Pow(x - 1, 2);
		}
	}

	public class EaseInOutBack : SomeInterpolate 
	{
        public override float Transform(float time)
        {
            float x = time;
            float c1 = 1.70158f;
            float c2 = c1 * 1.525f;
            return x < 0.5
            ? (MathF.Pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
            : (MathF.Pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
        }
    }

	public class _InterpolateType {
		public static SomeInterpolate linear = new LinearInterpolate();
		public static SomeInterpolate easeInSquared = new EaseInSquared();
		public static SomeInterpolate easeInCubed = new EaseInCubed();
		public static SomeInterpolate easeOutSquared = new EaseOutSquared();
		public static SomeInterpolate easeOutCubed = new EaseOutCubed();
		public static SomeInterpolate easeOutElastic = new EaseOutElastic();
		public static SomeInterpolate easeInBack = new EaseInBack();
		public static SomeInterpolate easeOutBack = new EaseOutBack();
		public static SomeInterpolate easeInOutBack = new EaseInOutBack();
	}
}
