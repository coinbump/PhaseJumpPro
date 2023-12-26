using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple dictionary
 * CODE REVIEW: 1/14/23
 */
namespace PJ
{
    /// <summary>
    /// Stores factories accessed by a class ID
    /// </summary>
    /// <typeparam name="Type"></typeparam>
    public class FactoryRegistry<Type> : Dictionary<string, Factory<Type>>
    {
        public virtual Type New(string id)
        {
            if (TryGetValue(id, out Factory<Type> factory))
            {
                var result = factory.New();

                // Make sure the object gets the classID when created
                if (null != result && result is Core)
                {
                    var coreResult = result as Core;
                    coreResult.ClassId = id;
                }

                return result;
            }

            return default(Type);
        }
    }

    /// <summary>
    /// Stores input factories accessed by a class ID
    /// </summary>
    /// <typeparam name="Type"></typeparam>
    public class InputFactoryRegistry<Input, Type> : Dictionary<string, InputFactory<Input, Type>>
    {
        public virtual Type New(string id, Input input)
        {
            if (TryGetValue(id, out InputFactory<Input, Type> factory))
            {
                return factory.New(input);
            }

            return default(Type);
        }
    }
}
