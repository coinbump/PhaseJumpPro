using System;
using System.Collections;

/*
 * RATING: 5 stars. Simple type.
 * CODE REVIEW: 4/11/22
 */
namespace PJ
{
    /// <summary>
    /// Key-value pair, independent of a dictionary
    /// </summary>
    public class Attribute<Key, Value> where Key : IComparable
    {
        public Key key;
        public Value value;

        public Attribute(Key key, Value value)
        {
            this.key = key;
            this.value = value;
        }

        public bool Equals(Attribute<Key, Value> b)
        {
            return key.Equals(b.key); // WRONG: && value == b.value;
        }
    }
}
