using System;

/*
 * RATING: 5 stars
 * Simple interface
 * CODE REVIEW: 4/10/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Interface for identifiable objects
    /// </summary>
    public interface StringIdentifiable
    {
        public abstract string Id { get; set; }
    }
}
