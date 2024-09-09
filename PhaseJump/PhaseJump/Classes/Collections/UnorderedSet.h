#pragma once

#include "CollectionUtils.h"
#include <iterator>
#include <unordered_set>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/3/24
 */
namespace PJ {
    /// Alias for std so can build an alternate implementation if needed
    template <
        class Type, class _Hash = std::hash<Type>, class _Pred = std::equal_to<Type>,
        class _Alloc = std::allocator<Type>>
    using UnorderedSet = std::unordered_set<Type, _Hash, _Pred, _Alloc>;

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

    // TODO: template <class Set, class Type> + requires
    template <class Type>
    UnorderedSet<Type> operator-(UnorderedSet<Type> const& lhs, UnorderedSet<Type> const& rhs) {
        UnorderedSet<Type> result;

        std::set_difference(
            lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::inserter(result, result.end())
        );

        return result;
    }
} // namespace PJ
