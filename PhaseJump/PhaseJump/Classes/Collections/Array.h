#ifndef PJARRAY_H
#define PJARRAY_H

#include "Macros_Collections.h"
#include <vector>
#include <algorithm>

/*
 RATING: 5 stars
 Adds convenience funcs to std::vector
 CODE REVIEW: 11/20/22
 */
namespace PJ {
    /// <summary>
    /// Extends std::vector with convenience methods
    /// </summary>
    template <class T> class Array : public std::vector<T> {
    public:
        using Base = std::vector<T>;

        // Convenience
        void Append(T const& value) { this->push_back(value); }
        void Add(T const& value) { this->push_back(value); }
        size_t Count() const { return this->size(); }

        // NOTE: Avoid Remove for large lists, it is inefficient
        COLLECTION_METHODS(T)
    };

    // Convenience names
    template <class T> using Vector = Array<T>;
}

#endif
