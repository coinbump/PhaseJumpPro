using System;
using Newtonsoft.Json;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 3/28/23
*/
namespace PJ
{
    /// <summary>
    /// Newtonsoft JSON deserializes unidentified integers as Int64, but most code uses Int32
    /// Add this to JSON settings to convert.
    /// IMPORTANT: Using the [] marking syntax above the dictionary in a codable will not call CanConvert and this will not work.
    /// </summary>
    public class Int64ToInt32JsonConverter : JsonConverter
    {
        /// <summary>
        /// Deserialize only
        /// </summary>
        public override bool CanWrite => false;

        /// <summary>
        /// Deserialize only
        /// </summary>
        public override void WriteJson(JsonWriter writer, object value, JsonSerializer serializer)
        {
            throw new NotImplementedException();
        }

        public override object ReadJson(JsonReader reader, Type objectType, object existingValue, JsonSerializer serializer)
        {
            return (reader.TokenType == JsonToken.Integer)
                ? Convert.ToInt32(reader.Value) // Convert to Int32 instead of Int64
                : serializer.Deserialize(reader);
        }

        public override bool CanConvert(Type objectType)
        {
            return objectType == typeof(Int64) ||
                objectType == typeof(object);
        }
    }
}
