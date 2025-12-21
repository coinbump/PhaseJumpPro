#pragma once

#include "OrderedSet.h"
#include "UnorderedSet.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/20/25
 */
namespace PJ {
    template <class Set>
    concept IsStdSet = std::is_same_v<Set, std::set<typename Set::value_type>>;

    template <class Set>
        requires IsStdSet<Set>
    void AddOrRemove(Set& set, typename Set::value_type const& value, bool add) {
        if (add) {
            set.insert(value);
        } else {
            set.erase(value);
        }
    }

    template <class Set>
        requires IsStdSet<Set>
    Set operator-(Set const& lhs, Set const& rhs) {
        Set result;

        std::set_difference(
            lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::inserter(result, result.begin())
        );

        return result;
    }

    /// Filters items from the collection. If check returns true, the item is kept
    /// @return Returns the filtered collection
    template <class Set, class UnaryPred>
        requires IsStdSet<Set>
    constexpr Set Filter(Set const& collection, UnaryPred check) {
        Set result;
        std::copy_if(
            begin(collection), end(collection), std::inserter(result, result.begin()), check
        );
        return result;
    }

    /// Maps a collection of one type to a collection of a different type
    template <class ResultType, class Type, class Converter>
    constexpr OrderedSet<ResultType> Map(OrderedSet<Type> const& collection, Converter convert) {
        OrderedSet<ResultType> result;
        std::transform(
            begin(collection), end(collection), std::inserter(result, result.begin()), convert
        );
        return result;
    }

    // MARK: UnorderedSet

    template <class Set>
    concept IsStdUnorderedSet = std::is_same_v<Set, std::unordered_set<typename Set::value_type>>;

    template <class Set>
        requires IsStdUnorderedSet<Set>
    void AddOrRemove(Set& set, typename Set::value_type const& value, bool add) {
        if (add) {
            set.insert(value);
        } else {
            set.erase(value);
        }
    }

    template <class Set>
        requires IsStdUnorderedSet<Set>
    Set operator-(Set const& lhs, Set const& rhs) {
        Set result = lhs;

        /// Can't use `std::set_difference`, it only works with ordered collections
        for (auto const& rightItem : rhs) {
            result.erase(rightItem);
        }

        return result;
    }

    /// Filters items from the collection. If check returns true, the item is kept
    /// @return Returns the filtered collection
    template <class Set, class UnaryPred>
        requires IsStdUnorderedSet<Set>
    constexpr UnorderedSet<typename Set::value_type>
    Filter(Set const& collection, UnaryPred check) {
        Set result;
        std::copy_if(
            begin(collection), end(collection), std::inserter(result, result.begin()), check
        );
        return result;
    }

    /// Maps a collection of one type to a collection of a different type
    template <class ResultType, class Type, class Converter>
    constexpr UnorderedSet<ResultType>
    Map(UnorderedSet<Type> const& collection, Converter convert) {
        UnorderedSet<ResultType> result;
        std::transform(
            begin(collection), end(collection), std::inserter(result, result.begin()), convert
        );
        return result;
    }
} // namespace PJ
