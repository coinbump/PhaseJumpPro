using UnityEngine;
using System.Collections.Generic;

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
        /// Outputs sin wave values
        /// </summary>
        public class SinMap1DNode : GoStandardAcyclicNode
        {
            public class Map : SomeMap1D
            {
                public bool isOutputNormal = false;
                public float offset = 0;

                public Map(bool isOutputNormal, float offset = 0)
                {
                    this.isOutputNormal = isOutputNormal;
                    this.offset = offset;
                }

                public override float Transform(float key)
                {
                    var result = Mathf.Sin((key + offset) * 2.0f * Mathf.PI);

                    if (isOutputNormal)
                    {
                        // Convert value range [-1, 1] to [0, 1.0]
                        result = (result + 1.0f) / 2.0f;
                    }

                    return result;
                }
            }

            private Map map;

            public SinMap1DNode(bool isOutputNormal, float offset = 0f)
            {
                map = new Map(isOutputNormal, offset);
            }

            public override object ValueForOutput<T>(string id, Tags parameters, T defaultValue)
            {
                if (typeof(T).IsAssignableFrom(typeof(SomeMap<float, float>)))
                {
                    return map;
                }
                else
                {
                    return defaultValue;
                }
            }

            public Map1DToMap2DNode ToMap2D(Axis2D axis)
            {
                var transformNode = new Map1DToMap2DNode(axis);
                AddOutput("value", transformNode, "value");
                return transformNode;
            }
        }
    }
}
