using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Has unit test
 * CODE REVIEW: 3/12/22
 */
namespace PJ
{
    namespace Graph
    {
        public abstract class SomeMap2DNode : GoStandardAcyclicNode
        {
        }

        /// <summary>
        /// Generates Perlin noise
        /// </summary>
        public class PerlinNoiseNode : SomeMap2DNode
        {
            private Noise2D.Perlin noise;

            public PerlinNoiseNode(Vector2 offset, Vector2 scale)
            {
                noise = new Noise2D.Perlin(offset, scale);
            }

            public override object ValueForOutput<T>(string id, Tags parameters, T defaultValue)
            {
                if (typeof(T).IsAssignableFrom(typeof(SomeMap<Vector2, float>)))
                {
                    return noise;
                }
                else
                {
                    return defaultValue;
                }
            }
        }
    }
}
