using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using PJ;

/*
 * RATING: 5 stars
 * Simple, has unit tests
 * Code review: 1/7/22
 * 
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Holds custom attributes (tags) in a dictionary
    /// </summary>
    public class Tags : Dictionary<string, object>
    {
        public Tags()
        {
        }

        public Tags(List<TagValue> tagValues)
        {
            foreach (TagValue tag in tagValues)
            {
                try
                {
                    var floatValue = float.Parse(tag.value);
                    this[tag.name] = floatValue;
                    continue;
                }
                catch
                {
                }

                this[tag.name] = tag.value;
            }
        }

        /// <summary>
        /// Creates a value if it doesn't exist
        /// </summary>
        public T SafeValue<T>(string key) where T : new()
        {
            try
            {
                T result = (T)this[key];
                return result;
            }
            catch (System.Exception e)
            {
                // Debug.Log(e.Message);
            }

            return new T();
        }

        public T SafeValue<T>(string key, Func<T> constructor)
        {
            try
            {
                T result = (T)this[key];
                return result;
            }
            catch (System.Exception e)
            {
                // Debug.Log(e.Message);
            }

            return constructor();
        }

        public PJ.Optional<T> Value<T>(string key)
        {
            try
            {
                T result = (T)this[key];
                return new Optional<T>(result);
            }
            catch
            {
            }

            return null;
        }

        public T DefaultValue<T>(string key, T defaultValue)
        {
            try
            {
                T result = (T)this[key];
                return result;
            }
            catch
            {
            }

            return defaultValue;
        }

        public T ParseValue<T>(string key, Func<string, T> parser, T defaultValue)
        {
            try
            {
                var value = this[key] as String;
                var result = parser(value);
                return result;
            }
            catch
            {
            }

            return defaultValue;
        }

        /// <summary>
        /// Returns true if the value exists, and is of the correct type
        /// </summary>
        public bool ContainsTypedValue<T>(string key)
        {
            try
            {
                T result = (T)this[key];
                return true;
            }
            catch
            {
            }

            return false;
        }
    }

    /// <summary>
    /// Used for generic storage of tag values
    /// </summary>
    [Serializable]
    public struct TagValue
    {
        public string name;
        public string value;    // Type is inferred from value

        public TagValue(string name, string value)
        {
            this.name = name;
            this.value = value;
        }
    }

    /// <summary>
    /// Holds a set of type tag strings
    ///
    /// Example: "enemy", "ghost" tags for an arcade game
    /// </summary>
    public class TypeTagsSet : HashSet<string>
    {
        public TypeTagsSet() : base()
        {
        }

        public TypeTagsSet(IEnumerable<string> collection) : base(collection)
        {
        }
    }
}
