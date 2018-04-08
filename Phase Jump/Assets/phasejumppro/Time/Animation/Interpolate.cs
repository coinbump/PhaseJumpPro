using System;
using System.Collections;
using NUnit.Framework;

/*
 * RATING: 5 stars. Works, missing some interpolation curves.
 * CODE REVIEW: Modern. 3.31.18
 */
namespace PJ
{
	/// <summary>
	/// Interpolate a normalized float.(0-1.0)
	/// Used for animation curves
	/// </summary>
	public abstract class Interpolate
	{
		public abstract float Transform(float factor);
	}

	public class InterpolateLinear : Interpolate
	{
		public override float Transform(float factor)
		{
			return factor;
		}
	}

	public class InterpolateSquared : Interpolate
	{
		public override float Transform(float factor)
		{
			return factor * factor;
		}
	}

	public class InterpolateCubed : Interpolate
	{
		public override float Transform(float factor)
		{
			return factor * factor * factor;
		}
	}

	public class InterpolateOutSquared : Interpolate
	{
		public override float Transform(float factor)
		{
			factor = 1.0f - factor;
			var result = 1.0f - (factor * factor);
			return result;
		}
	}

	public class InterpolateOutCubed : Interpolate
	{
		public override float Transform(float factor)
		{
			factor = 1.0f - factor;
			var result = 1.0f - (factor * factor * factor);
			return result;
		}
	}

	// FUTURE: evaluate this curve.
	public class InterpolateLeadUp : Interpolate
	{
		public override float Transform(float factor)
		{
			// Lead up before ease in
			var s = factor;
			var result = (s + 1.0f) * (factor * factor * factor) - (s * factor * factor);
			return result;
		}
	}

	// FUTURE: evaluate this curve.
	public class InterpolateOvershootOut : Interpolate
	{
		public override float Transform(float factor)
		{
			// Go past final value, then return
			var s = factor;
			factor = 1.0f - factor;
			var result = 1.0f - ((s + 1.0f) * (factor * factor * factor) - (s * factor * factor));
			return result;
		}
	}

	public class UnitTests_Interpolate {

		[Test]
		public void UnitTests()
		{
			{
				var i = new InterpolateLinear();
				Assert.LessOrEqual(i.Transform(1.0f), 1.0f);
			}
			{
				var i = new InterpolateSquared();
				Assert.LessOrEqual(i.Transform(.25f), .25f*.25f);
			}
			{
				var i = new InterpolateCubed();
				Assert.LessOrEqual(i.Transform(.25f), (.25f * .25f * .25f));
			}
			// No more unit tests needed
		}
	}
}

