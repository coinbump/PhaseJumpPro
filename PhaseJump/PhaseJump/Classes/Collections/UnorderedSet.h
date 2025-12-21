#pragma once

#include <algorithm>
#include <iterator>
#include <unordered_set>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/3/24
 */
namespace PJ {
    /// Alias for standard type so you can build an alternate implementation if needed
    template <
        class Type, class _Hash = std::hash<Type>, class _Pred = std::equal_to<Type>,
        class _Alloc = std::allocator<Type>>
    using UnorderedSet = std::unordered_set<Type, _Hash, _Pred, _Alloc>;
} // namespace PJ
