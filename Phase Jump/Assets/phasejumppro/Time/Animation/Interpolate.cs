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
	abstract class Interpolate
	{
		public abstract float Transform(float factor);
	}

	class InterpolateLinear : Interpolate
	{
		public override float Transform(float factor)
		{
			return factor;
		}
	}

	class InterpolateSquared : Interpolate
	{
		public override float Transform(float factor)
		{
			return factor * factor;
		}
	}

	class InterpolateCubed : Interpolate
	{
		public override float Transform(float factor)
		{
			return factor * factor * factor;
		}
	}

	class InterpolateOutSquared : Interpolate
	{
		public override float Transform(float factor)
		{
			factor = 1.0f - factor;
			var result = 1.0f - (factor * factor);
			return result;
		}
	}

	class InterpolateOutCubed : Interpolate
	{
		public override float Transform(float factor)
		{
			factor = 1.0f - factor;
			var result = 1.0f - (factor * factor * factor);
			return result;
		}
	}

	// FUTURE: evaluate this curve.
	class InterpolateLeadUp : Interpolate
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
	class InterpolateOvershootOut : Interpolate
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

	class Interpolate_UnitTests {

		[Test]
		public void UnitTests()
		{
			{
				var i = new InterpolateLinear();
				Assert.LessOrEqual(Math.Abs(i.Transform(1.0f) - 1.0f), .001f);
			}
			{
				var i = new InterpolateSquared();
				Assert.LessOrEqual(Math.Abs(i.Transform(.25f) - (.25f*.25f)), .001f);
			}
			{
				var i = new InterpolateCubed();
				Assert.LessOrEqual(Math.Abs(i.Transform(.25f) - (.25f * .25f * .25f)), .001f);
			}
			// No more unit tests needed
		}
	}
}

