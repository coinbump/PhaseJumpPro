using Godot;
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

        public Tags(Dictionary<string, object> tags) : base(tags)
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
        public T SafeValue<T>(string key, T defaultValue)
        {
            return SafeValue<T>(key, () => defaultValue);
        }

        public T SafeValue<T>(string key, Func<T> allocator)
        {
            try
            {
                T result = (T)this[key];
                return result;
            }
            catch //(System.Exception e)
            {
                // GD.Print(e.Message);
            }

            return allocator();
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

        /// <summary>
        /// Convert types of values.
        /// Example: Decode as Int64, but represent as int
        /// </summary>
        public Tags ValueTypesConverted<Left, Right>(Func<Left, Right> converter)
        {
            Tags result = new();

            foreach ((var key, var value) in this)
            {
                try
                {
                    if (value is Left)
                    {
                        result[key] = converter((Left)value);
                        continue;
                    }
                }
                catch
                {
                }

                result[key] = value;
            }

            return result;
        } // TESTED

        /// <summary>
        /// Add the optional's value to the dictionary if it exists, remove the value if the optional is null
        /// </summary>
        public void SetOptionalValue<T>(String key, Optional<T> value)
        {
            if (null != value)
            {
                this[key] = value.value;
            }
            else
            {
                this.Remove(key);
            }
        } // TESTED
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

        public void Set(string tag, bool value)
        {
            if (value)
            {
                Add(tag);
            }
            else
            {
                Remove(tag);
            }
        }
    }
}
