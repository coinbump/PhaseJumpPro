#pragma once

#include "Macros.h"
#include <algorithm>
#include <set>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/3/24
 */
namespace PJ {
    /// Alias for standard type so you can build an alternate implementation if needed
    template <class Type, class _Compare = std::less<Type>, class _Allocator = std::allocator<Type>>
    using OrderedSet = std::set<Type, _Compare, _Allocator>;

    /// Used to store weak pointers, which require a custom compare operator
    template <class Type, class _Allocator = std::allocator<Type>>
    using WeakOrderedSet = OrderedSet<Type, std::owner_less<Type>, _Allocator>;

    // TODO: needs unit test
    /// Remove all expired weak pointers in the set
    template <class WeakOrderedSetType>
    WeakOrderedSetType Refreshed(WeakOrderedSetType const& set) {
        WeakOrderedSetType result;
        for (auto& e : set) {
            GUARD_CONTINUE(!e.expired())
            result.insert(e.lock());
        }
        return result;
    }
} // namespace PJ
