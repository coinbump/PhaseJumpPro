using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple dictionary
 * CODE REVIEW: 1/14/23
 */
namespace PJ
{
    /// <summary>
    /// Store factories accessed by a class ID
    /// </summary>
    /// <typeparam name="Type"></typeparam>
    public class FactoryRegistry<Type> : Dictionary<string, Factory<Type>>
    {
        public Type New(string id)
        {
            if (TryGetValue(id, out Factory<Type> factory))
            {
                return factory.New();
            }

            return default(Type);
        }
    }
}
