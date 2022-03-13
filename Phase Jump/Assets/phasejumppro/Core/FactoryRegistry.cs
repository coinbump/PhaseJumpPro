using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple dictionary
 * CODE REVIEW: 1/11/22
 */
namespace PJ
{
    /// <summary>
    /// Store factories accessed by a class ID
    /// </summary>
    /// <typeparam name="Type"></typeparam>
    public class FactoryRegistry<Type> : Dictionary<string, Factory<Type>>
    {

    }
}
