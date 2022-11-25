using UnityEngine;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 3/12/22
 * 
 * Ported to: C++
 */
namespace PJ
{
    /// <summary>
    /// A class that transforms a value
    /// </summary>
    public abstract class SomeTransform<Value>
    {
        public abstract Value Transform(Value value);
    }

    public class IdentityTransform<Value> : SomeTransform<Value>
    {
        public override Value Transform(Value value)
        {
            return value;
        }
    }
}
