#ifndef PJVECTORLIST_H
#define PJVECTORLIST_H

#include "Macros_Collections.h"
#include <vector>

/*
 RATING: 5 stars
 Has unit tests (in ListTests)
 CODE REVIEW: 11/20/22
 */
namespace PJ {
    /// <summary>
    /// Extends std::vector with convenience methods
    /// Called VectorList to avoid confusion with Vector geometry objects.
    /// </summary>
    template <class T, class Allocator = std::allocator<T>> class VectorList : public std::vector<T> {
    public:
        using Base = std::vector<T>;

        VectorList() {
        }

        constexpr VectorList(std::initializer_list<T> init,
                        const Allocator& alloc = Allocator()) : Base(init, alloc) {
        }

        // Convenience
        void Append(T const& value) { this->push_back(value); }
        void Add(T const& value) { this->push_back(value); }

        // NOTE: Avoid Remove for large lists, it is inefficient
        COLLECTION_METHODS(VectorList<T>, T)
    };

    // Convenience names
    template <class T> using Vector = VectorList<T>;
    template <class T> using VL = VectorList<T>;
}

#endif
