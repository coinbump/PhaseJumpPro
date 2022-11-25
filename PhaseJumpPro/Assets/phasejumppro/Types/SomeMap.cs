using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 3/12/22
 * 
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// A class that maps a key to a value
    /// </summary>
    public abstract class SomeMap<Key, Value>
    {
        public abstract Value ValueFor(Key key);

        public Value this[Key key]
        {
            get { return ValueFor(key); }
        }
    }
}
