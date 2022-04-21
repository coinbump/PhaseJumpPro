using System.Collections.Generic;

namespace PJ
{
	public class InterpolateClass
	{
		/// <summary>
		/// Registers classes for each interpolate type
		/// </summary>
		public class FactoryRegistry
		{
			public static Dictionary<string, SomeFactory> registry = new Dictionary<string, SomeFactory>();

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
				if (registry.TryGetValue(type, out SomeFactory registryValue))
				{
					var element = registryValue;

					var result = element.NewObject() as Interpolate;
					return result;
				}

				return null;
			}
		}
	}
}
