using System;
using UnityEngine;

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
        /// Multiplies values coming from a Map2D by a factor
        /// </summary>
        public class MultiplyMap2DNode : SomeMap2DNode
        {
            public float factor = 1.0f;

            public MultiplyMap2DNode(float factor = 1.0f)
            {
                this.factor = factor;
            }

            public override object ValueForOutput<T>(string id, Tags parameters, T defaultValue)
            {
                if (typeof(T).IsAssignableFrom(typeof(SomeTransform<Vector2, float>)))
                {
                    var inputValue = ValueForInput<SomeTransform<Vector2, float>>("value", new Tags(), null);
                    if (null != inputValue)
                    {
                        var inputMap = inputValue as SomeTransform<Vector2, float>;
                        return new KeyValueMapsMap<Vector2, float>(key => inputMap.ValueFor(key), value => value * factor);
                    }
                }

                return defaultValue;
            }
        }
    }
}
