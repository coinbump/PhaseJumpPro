using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 3/13/22
 */
namespace PJ
{
    namespace Graph
    {
        /// <summary>
        /// Returns inside/outside value based on position in circle
        /// Future: support fallof, with gradation
        /// </summary>
        public class CircleMap2DNode : SomeMap2DNode
        {
            private class Map : SomeMap<Vector2, float>
            {
                public Vector2 center = Vector2.zero;
                public float radius = 1.0f;
                public float insideValue = 1.0f;
                public float outsideValue = 0;

                public Map(Vector2 center, float radius, float insideValue, float outsideValue)
                {
                    this.center = center;
                    this.radius = radius;
                    this.insideValue = insideValue;
                    this.outsideValue = outsideValue;
                }

                public override float ValueFor(Vector2 key)
                {
                    var xDelta = key.x - center.x;
                    var yDelta = key.y - center.y;

                    var distance = Mathf.Sqrt(xDelta * xDelta + yDelta * yDelta);
                    return distance <= radius ? insideValue : outsideValue;
                }
            }

            private Map map;

            public CircleMap2DNode(Vector2 center, float radius, float insideValue = 1.0f, float outsideValue = 0.0f)
            {
                map = new Map(center, radius, insideValue, outsideValue);
            }

            public override object ValueForOutput<T>(string identifier, Tags parameters, T defaultValue)
            {
                if (typeof(T).IsAssignableFrom(typeof(SomeMap<Vector2, float>)))
                {
                    return map;
                }
                else
                {
                    return defaultValue;
                }
            }
        }
    }
}
