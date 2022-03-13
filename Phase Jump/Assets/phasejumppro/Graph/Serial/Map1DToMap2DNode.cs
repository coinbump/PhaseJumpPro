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
        /// Convert a 1D map to a map in 2D space
        /// </summary>
        public class Map1DToMap2DNode : GoStandardNode
        {
            public Axis2D axis;

            public Map1DToMap2DNode(Axis2D axis)
            {
                this.axis = axis;
            }

            protected class Map : SomeMap<Vector2, float>
            {
                public Map1D map1D;
                public Axis2D axis;

                public Map(Map1D map1D, Axis2D axis)
                {
                    this.map1D = map1D;
                    this.axis = axis;
                }

                public override float ValueFor(Vector2 key)
                {
                    switch (axis)
                    {
                        case Axis2D.X:
                            return map1D.ValueFor(key.x);
                        default:
                            return map1D.ValueFor(key.y);
                    }
                }
            }

            public Map1DToMap2DNode()
            {
            }

            public override object ValueForOutput<T>(string identifier, Tags parameters, T defaultValue)
            {
                if (typeof(T).IsAssignableFrom(typeof(SomeMap<Vector2, float>)))
                {
                    var inputValue = ValueForInput<SomeMap<float, float>>("value", null, null);
                    if (null != inputValue)
                    {
                        return new Map(inputValue as Map1D, axis);
                    }
                }

                return defaultValue;
            }
        }
    }
}
