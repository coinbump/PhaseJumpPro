using UnityEngine;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 3/12/22
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
