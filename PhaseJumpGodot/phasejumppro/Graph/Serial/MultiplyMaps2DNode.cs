using System;
using Godot;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 3/12/22
 */
namespace PJ
{
    namespace Graph
    {
        /// <summary>
        /// Multiplies values from 2 Map2Ds
        /// </summary>
        public class MultiplyMaps2DNode : GoStandardAcyclicNode
        {
            public float factor = 1.0f;

            public override object ValueForOutput<T>(string id, Tags parameters, T defaultValue)
            {
                if (typeof(T).IsAssignableFrom(typeof(SomeTransform<Vector2, float>)))
                {
                    var inputValue1 = ValueForInput<SomeTransform<Vector2, float>>("map-1", new Tags(), null);
                    var inputValue2 = ValueForInput<SomeTransform<Vector2, float>>("map-2", new Tags(), null);
                    if (null != inputValue1 && null != inputValue2)
                    {
                        var inputMap1 = inputValue1 as SomeTransform<Vector2, float>;
                        var inputMap2 = inputValue2 as SomeTransform<Vector2, float>;
                        return new KeyValueMapsMap<Vector2, float>(key => inputMap1.ValueFor(key) * inputMap2.ValueFor(key), value => value);
                    }
                }

                return defaultValue;
            }
        }
    }
}
