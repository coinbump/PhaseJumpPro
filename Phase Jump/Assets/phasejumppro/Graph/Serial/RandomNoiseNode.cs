using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 3/12/22
 */
namespace PJ
{
    namespace Graph
    {
        // <summary>
        // Generates random noise
        // </summary>
        public class RandomNoiseNode : GoStandardNode
        {
            private Noise2D.Random noise = new Noise2D.Random();

            public override object ValueForOutput<T>(string identifier, Tags parameters, T defaultValue)
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
