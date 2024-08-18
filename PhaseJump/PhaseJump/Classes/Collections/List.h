#pragma once

#include "CollectionUtils.h"
#include <list>
#include <memory>

// TODO: rethink using inheritance here
/*
 RATING: 5 stars
 Extends std::list
 CODE REVIEW: 6/8/24
 */
namespace PJ {
    /// Extends std::list with convenience methods
    template <class T, class Allocator = std::allocator<T>>
    class List : public std::list<T> {
    public:
        using Base = std::list<T>;
        using This = List<T>;

        List() {}

        constexpr List(std::initializer_list<T> init, const Allocator& alloc = Allocator()) :
            Base(init, alloc) {}

        // Convenience
        void Append(T const& value) {
            this->push_back(value);
        }

        void Add(T const& value) {
            this->push_back(value);
        }
    };
} // namespace PJ
