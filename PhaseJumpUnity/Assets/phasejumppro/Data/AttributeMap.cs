using System;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/11/22
 */
namespace PJ
{
    /// <summary>
    /// Map of Attribute objects
    /// Useful for some encoding/decoding scenarios
    /// </summary>
    public class AttributeMap : Dictionary<string, StringsAttribute>
    {
        public void AddAttribute(string key, string value)
        {
            Add(key, new StringsAttribute(key, value));
        }

        public bool BoolValue(string key, bool defaultVal)
        {
            var attribute = this[key];
            if (null == attribute) { return defaultVal; }

            return attribute.value == "1" || attribute.value == "true" || attribute.value == "t";
        }

        public int IntValue(string key, int defaultVal)
        {
            var attribute = this[key];
            if (null == attribute) { return defaultVal; }

            var result = Convert.ToInt32(attribute.value);
            return result;
        }

        public Int64 Int64Value(string key, Int64 defaultVal)
        {
            var attribute = this[key];
            if (null == attribute) { return defaultVal; }

            var result = Convert.ToInt64(attribute.value);
            return result;
        }

        public float FloatValue(string key, float defaultVal)
        {
            var attribute = this[key];
            if (null == attribute) { return defaultVal; }

            var result = (float)Convert.ToDouble(attribute.value);
            return result;
        }

        public UInt32 UIntValue(string key, UInt32 defaultVal)
        {
            var attribute = this[key];
            if (null == attribute) { return defaultVal; }

            var result = Convert.ToUInt32(attribute.value);
            return result;
        }

        public string StringValue(string key, string defaultVal)
        {
            var attribute = this[key];
            if (null == attribute) { return defaultVal; }

            var result = attribute.value;
            return result;
        }
    }
}
