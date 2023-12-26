using Godot;
using System.Collections;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 3/12/22
 */
namespace PJ
{
    namespace Noise2D
    {
        /// <summary>
        /// Simple noise function: random value at each position
        /// </summary>
        public class Random : SomeTransform<Vector2, float>
        {
            public override float Transform(Vector2 key)
            {
                return UnityEngine.Random.Range(0, 1.0f);
            }
        }

        /// <summary>
        /// Simple noise function: random value at each position
        /// </summary>
        public class Perlin : SomeTransform<Vector2, float>
        {
            public Vector2 origin;
            public Vector2 scale;

            public Perlin(Vector2 origin, Vector2 scale)
            {
                this.origin = origin;
                this.scale = scale;
            }

            public override float Transform(Vector2 key)
            {
                return Mathf.PerlinNoise(origin.X + (key.X * scale.X), origin.Y + (key.Y * scale.Y));
            }
        }
    }
}
