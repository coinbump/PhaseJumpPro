using System;
using System.Collections.Generic;

/*
 * RATING: 4 stars. Works, missing some interpolation curves.
 * CODE REVIEW: Modern. 3.31.18
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
	/// Used for animation curves
	///
	/// Analogous to Unity's AnimationCurve, but with more built-in equation curves
	/// </summary>
	public class Interpolate
	{
		public virtual float Evaluate(float time) { return time; }

		/// <summary>
		/// Registers classes for each interpolate type
		/// </summary>
		public class FactoryRegistry
		{
			public static Dictionary<string, AnyFactory> registry = new Dictionary<string, AnyFactory>();

			public FactoryRegistry()
			{
                registry.Add(InterpolateType.Linear.ToString().ToLower(), new FactoryNew<InterpolateLinear>());
				registry.Add(InterpolateType.Squared.ToString().ToLower(), new FactoryNew<InterpolateSquared>());
				registry.Add(InterpolateType.Cubed.ToString().ToLower(), new FactoryNew<InterpolateCubed>());
				registry.Add(InterpolateType.OutSquared.ToString().ToLower(), new FactoryNew<InterpolateOutSquared>());
				registry.Add(InterpolateType.OutCubed.ToString().ToLower(), new FactoryNew<InterpolateOutCubed>());
			}

			public Interpolate New(InterpolateType type)
			{
				return New(type.ToString().ToLower());
			}

			public Interpolate New(string type)
			{
				if (type.Length <= 0) { return null; }

				var registryValue = registry[type];
				var element = registryValue as SomeFactory;
				if (null == element) { return null; }

				var result = element.NewObject() as Interpolate;
				return result;
			}
		}
		static public FactoryRegistry factory = new FactoryRegistry();
	}

	public class InterpolateLinear : Interpolate
	{
		public override float Evaluate(float time)
		{
			return time;
		}
	}

	public class InterpolateSquared : Interpolate
	{
		public override float Evaluate(float time)
		{
			return time * time;
		}
	}

	public class InterpolateCubed : Interpolate
	{
		public override float Evaluate(float time)
		{
			return time * time * time;
		}
	}

	public class InterpolateOutSquared : Interpolate
	{
		public override float Evaluate(float time)
		{
			time = 1.0f - time;
			var result = 1.0f - (time * time);
			return result;
		}
	}

	public class InterpolateOutCubed : Interpolate
	{
		public override float Evaluate(float time)
		{
			time = 1.0f - time;
			var result = 1.0f - (time * time * time);
			return result;
		}
	}

	// FUTURE: evaluate this curve (is the equation correct?)
	public class InterpolateLeadUp : Interpolate
	{
		public override float Evaluate(float time)
		{
			// Lead up before ease in
			var s = time;
			var result = (s + 1.0f) * (time * time * time) - (s * time * time);
			return result;
		}
	}

	// FUTURE: evaluate this curve (is the equation correct?)
	public class InterpolateOvershootOut : Interpolate
	{
		public override float Evaluate(float time)
		{
			// Go past final value, then return
			var s = time;
			time = 1.0f - time;
			var result = 1.0f - ((s + 1.0f) * (time * time * time) - (s * time * time));
			return result;
		}
	}
}
