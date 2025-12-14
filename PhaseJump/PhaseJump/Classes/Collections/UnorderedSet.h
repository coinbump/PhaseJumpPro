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

    // TODO: use std::requires
    template <
        class Set, class Type,
        std::enable_if_t<std::is_same<
            std::unordered_set<
                typename Set::key_type, typename Set::hasher, typename Set::key_equal,
                typename Set::allocator_type>,
            Set>::value>* = nullptr>
    void AddOrRemove(Set& set, Type const& value, bool add) {
        if (add) {
            set.insert(value);
        } else {
            set.erase(value);
        }
    }

    template <class Type>
    UnorderedSet<Type> operator-(UnorderedSet<Type> const& lhs, UnorderedSet<Type> const& rhs) {
        UnorderedSet<Type> result = lhs;

        /// Can't use `std::set_difference`, it only works with ordered collections
        for (auto const& rightItem : rhs) {
            result.erase(rightItem);
        }

        return result;
    }
} // namespace PJ
