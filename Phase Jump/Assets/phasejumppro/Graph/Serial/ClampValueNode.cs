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
        /// Clamps float value and passes it along
        /// </summary>
        public class ClampValueNode : GoStandardNode
        {
            public float minValue;
            public float maxValue;

            public ClampValueNode(float minValue, float maxValue)
            {
                this.minValue = minValue;
                this.maxValue = maxValue;
            }

            public override object ValueForOutput<T>(string identifier, Tags parameters, T defaultValue)
            {
                if (typeof(T).IsAssignableFrom(typeof(float)))
                {
                    var inputValue = ValueForInput<float>("value", null, 0);
                    if (null != inputValue)
                    {
                        return Mathf.Clamp((float)inputValue, minValue, maxValue);
                    }
                }

                return defaultValue;
            }
        }
    }
}
