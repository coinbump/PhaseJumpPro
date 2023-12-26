using System.Collections.Generic;

namespace PJ
{
    public class InterpolateClass
    {
        /// <summary>
        /// Registers classes for each interpolate type
        /// </summary>
        public class MyRegistry
        {
            public static Dictionary<string, SomeFactory> registry = new Dictionary<string, SomeFactory>();

            public MyRegistry()
            {
                registry.Add(InterpolateType.Linear.ToString().ToLower(), new FactoryNew<LinearInterpolate>());
                registry.Add(InterpolateType.InSquared.ToString().ToLower(), new FactoryNew<EaseInSquared>());
                registry.Add(InterpolateType.InCubed.ToString().ToLower(), new FactoryNew<EaseInCubed>());
                registry.Add(InterpolateType.OutSquared.ToString().ToLower(), new FactoryNew<EaseOutSquared>());
                registry.Add(InterpolateType.OutCubed.ToString().ToLower(), new FactoryNew<EaseOutCubed>());
            }

            public SomeInterpolate New(InterpolateType type)
            {
                return New(type.ToString().ToLower());
            }

            public SomeInterpolate New(string type)
            {
                if (registry.TryGetValue(type, out SomeFactory registryValue))
                {
                    var element = registryValue;

                    var result = element.NewObject() as SomeInterpolate;
                    return result;
                }

                return null;
            }
        }
    }
}
