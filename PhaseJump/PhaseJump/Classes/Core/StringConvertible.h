#ifndef PJSTRINGCONVERTIBLE_H
#define PJSTRINGCONVERTIBLE_H

#include "_String.h"
#include <memory>

/*
 RATING: 5 stars
 Simple base class
 CODE REVIEW: 12/20/22
 */
namespace PJ {
    /// <summary>
    /// Can be converted to a string for logging
    /// </summary>
    class StringConvertible {
        virtual String ToString() const = 0;

        // Convenience
        String Description() const { return ToString(); }
    };
}

#endif
