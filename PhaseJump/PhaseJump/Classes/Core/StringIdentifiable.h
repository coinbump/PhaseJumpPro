#ifndef PJSTRINGIDENTIFIABLE_H
#define PJSTRINGIDENTIFIABLE_H

#include "_String.h"

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 8/3/24
 PORTED TO: C++, C#
 */
namespace PJ {
    /// Interface for identifiable objects
    class StringIdentifiable {
    public:
        virtual String Id() const = 0;
    };
} // namespace PJ

#endif
