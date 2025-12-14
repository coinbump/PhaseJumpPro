using UnityEngine;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 2/18/23
 * Ported to: C++
 */
namespace PJ
{
    /// <summary>
    /// A class that transforms a value
    /// </summary>
    public abstract class SomeTransform<Value, Result>
    {
        public abstract Result Transform(Value value);

        public Result ValueFor(Value value) { return Transform(value); }

        public Result this[Value key]
        {
            get { return Transform(key); }
        }
    }

    public abstract class SomeValueTransform<Value> : SomeTransform<Value, Value>
    {
    }

    public class IdentityTransform<Value> : SomeValueTransform<Value>
    {
        public override Value Transform(Value value)
        {
            return value;
        }
    }
}
