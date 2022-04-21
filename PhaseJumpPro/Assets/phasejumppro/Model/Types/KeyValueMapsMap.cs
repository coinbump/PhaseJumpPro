using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 3/12/22
 */
namespace PJ
{
    /// <summary>
    /// Allows for a transformation from one map to another via a mapping func
    /// </summary>
    public class KeyValueMapsMap<Key, Value> : SomeMap<Key, Value>
    {
        public Func<Key, Value> keyMap;
        public Func<Value, Value> valueMap;

        public KeyValueMapsMap(Func<Key, Value> keyMap, Func<Value, Value> valueMap)
        {
            this.keyMap = keyMap;
            this.valueMap = valueMap;
        }

        public override Value ValueFor(Key key)
        {
            var value = keyMap(key);
            var mappedValue = valueMap(value);
            return mappedValue;
        }
    }
}
