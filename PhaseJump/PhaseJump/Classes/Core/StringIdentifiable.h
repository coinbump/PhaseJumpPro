#ifndef PJSTRINGIDENTIFIABLE_H
#define PJSTRINGIDENTIFIABLE_H

#include "_String.h"

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
    class StringIdentifiable
    {
    public:
        virtual String Id() const = 0;
    };
}

#endif
