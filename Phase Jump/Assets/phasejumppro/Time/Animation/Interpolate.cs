using System;
using System.Collections.Generic;

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
	/// TODO: can this be integrated with Unity's AnimationCurve?
	public class Interpolate
	{
		public virtual float Transform(float factor) { return factor; }

		static FactoryRegistry factories = new FactoryRegistry();

		public class Factory<T> : PJ.AnyFactory where T: Interpolate, new()
		{
			public Interpolate New() { return new T(); }
		}
	}

	// For editors to pick from list
	public enum InterpolateType
	{
		Linear, Squared, Cubed, OutSquared, OutCubed
	}

	/// <summary>
	/// Registers factories for each interpolate type
	/// </summary>
	public class FactoryRegistry
	{
		// TODO: re-evaluate this.
		//public Dictionary<string, AnyFactory> registry = new Dictionary<string, AnyFactory>();
		//public FactoryRegistry()
		//{
		//	registry.Add(InterpolateType.Linear.ToString(), new Interpolate.Factory<InterpolateLinear>());
		//	registry.Add(InterpolateType.Squared.ToString(), new Interpolate.Factory<InterpolateSquared>());
		//	registry.Add(InterpolateType.Cubed.ToString(), new Interpolate.Factory<InterpolateCubed>());
		//	registry.Add(InterpolateType.OutSquared.ToString(), new Interpolate.Factory<InterpolateOutSquared>());
		//	registry.Add(InterpolateType.OutCubed.ToString(), new Interpolate.Factory<InterpolateOutCubed>());
		//}

		//public Interpolate New(InterpolateType type)
		//{
		//	var element = registry[type.ToString()];
		//	if (null == element) { return new Interpolate(); }
		//	return element.New();
		//}
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

}

