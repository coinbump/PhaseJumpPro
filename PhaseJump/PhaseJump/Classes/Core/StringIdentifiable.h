#ifndef PJSTRINGIDENTIFIABLE_H
#define PJSTRINGIDENTIFIABLE_H

#include <string>

/*
 * RATING: 5 stars
 * Simple interface
 * CODE REVIEW: 11/7/22
 */
namespace PJ
{
    /// <summary>
    /// Interface for identifiable objects
    /// </summary>
    public class StringIdentifiable
    {
        public virtual std::string Id() = 0;
    }
}

#endif
