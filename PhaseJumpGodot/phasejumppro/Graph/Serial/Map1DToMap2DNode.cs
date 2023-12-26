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
        /// Convert a 1D map to a map in 2D space
        /// </summary>
        public class Map1DToMap2DNode : GoStandardAcyclicNode
        {
            public Axis2D axis;

            public Map1DToMap2DNode(Axis2D axis)
            {
                this.axis = axis;
            }

            protected class Map : SomeTransform<Vector2, float>
            {
                public SomeMap1D map1D;
                public Axis2D axis;

                public Map(SomeMap1D map1D, Axis2D axis)
                {
                    this.map1D = map1D;
                    this.axis = axis;
                }

                public override float Transform(Vector2 key)
                {
                    switch (axis)
                    {
                        case Axis2D.X:
                            return map1D.Transform(key.X);
                        default:
                            return map1D.Transform(key.Y);
                    }
                }
            }

            public Map1DToMap2DNode()
            {
            }

            public override object ValueForOutput<T>(string id, Tags parameters, T defaultValue)
            {
                if (typeof(T).IsAssignableFrom(typeof(SomeTransform<Vector2, float>)))
                {
                    var inputValue = ValueForInput<SomeTransform<float, float>>("value", null, null);
                    if (null != inputValue)
                    {
                        return new Map(inputValue as SomeMap1D, axis);
                    }
                }

                return defaultValue;
            }
        }
    }
}
