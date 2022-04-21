using UnityEngine;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 3/12/22
 */
namespace PJ
{
    /// <summary>
    /// A class that transforms a value
    /// </summary>
    public abstract class SomeTransform<Value> : SomeMap<Value, Value>
    {
        public abstract Value Transform(Value value);

        public override Value ValueFor(Value key)
        {
            return Transform(key);
        }
    }
}
