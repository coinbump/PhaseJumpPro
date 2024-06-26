#ifndef PJLIST_H
#define PJLIST_H

#include "Macros_Collections.h"
#include <list>
#include <memory>

/*
 RATING: 5 stars
 Adds convenience funcs to std::vector
 CODE REVIEW: 11/20/22
 */
namespace PJ {
    /// <summary>
    /// Extends std::list with convenience methods
    /// </summary>
    template <class T, class Allocator = std::allocator<T>>
    class List : public std::list<T> {
    public:
        using Base = std::list<T>;
        using This = List<T>;

        List() {}

        constexpr List(std::initializer_list<T> init,
                       const Allocator& alloc = Allocator()) : Base(init, alloc) {
        }

        // Convenience
        void Append(T const& value) { this->push_back(value); }
        void Add(T const& value) { this->push_back(value); }

        COLLECTION_METHODS(List<T>, T)
    };
}

#endif
