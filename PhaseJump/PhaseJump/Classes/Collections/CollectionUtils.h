#pragma once

#include "InfixOStreamIterator.h"
#include "OrderedSet.h"
#include "StringUtils.h"
#include "UnorderedSet.h"
#include "Utils.h"
#include "VectorList.h"
#include <algorithm>
#include <functional>
#include <optional>
#include <sstream>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/31/24
 */
namespace PJ {
    /// Removes all elements from the collection
    template <class Collection>
    constexpr void RemoveAll(Collection& collection) {
        collection.clear();
    }

    /// @return Returns the number of elements in the collection
    template <class Collection>
    constexpr Collection::size_type Count(Collection const& collection) {
        return collection.size();
    }

    /// @return Returns true if the collection is empty
    template <class Collection>
    constexpr bool IsEmpty(Collection const& collection) {
        return collection.size() <= 0;
    }

    /// Removes the first element that matches the specified value
    template <class Collection>
    constexpr void Remove(Collection& collection, typename Collection::value_type const& value) {
        auto i = std::find(collection.begin(), collection.end(), value);
        GUARD(i != collection.end())

        collection.erase(i);
    }

    /// Removes the first element for which the check returns true
    template <class Collection, class UnaryPred>
    constexpr void RemoveFirstIf(Collection& collection, UnaryPred check) {
        auto i = std::find_if(collection.begin(), collection.end(), check);
        GUARD(i != collection.end())

        collection.erase(i);
    }

    /// Removes all elements for which the check returns true
    template <class Collection, class UnaryPred>
    constexpr void RemoveIf(Collection& collection, UnaryPred check) {
        collection.erase(
            std::remove_if(collection.begin(), collection.end(), check), collection.end()
        );
    }

    /// Removes all null items from the collection
    template <class Collection>
    constexpr void Compact(Collection& collection) {
        RemoveIf(collection, [](auto& item) { return !item; });
    }

    /// @return Returns true if the collection contains the specified value
    template <class Collection>
    constexpr bool
    Contains(Collection const& collection, typename Collection::value_type const& value) {
        auto findResult = std::find(collection.begin(), collection.end(), value);
        return findResult != collection.end();
    }

    /// @return Returns true if the collection contains a value where check returns true
    template <class Collection, class UnaryPred>
    constexpr bool ContainsIf(Collection const& collection, UnaryPred check) {
        auto i = std::find_if(collection.begin(), collection.end(), check);
        return i != collection.end();
    }

    /// Adds one collection to another
    template <class Collection>
    constexpr void AddRange(Collection& collection, Collection const& source) {
        collection.insert(collection.end(), source.begin(), source.end());
    }

    /// @return Returns the index of the matching item in the collection, if any
    template <class Collection>
    std::optional<size_t> constexpr IndexOf(
        Collection const& collection, typename Collection::value_type const& item
    ) {
        auto i = std::find(collection.begin(), collection.end(), item);

        GUARDR(i != collection.end(), {});
        return std::distance(collection.begin(), i);
    }

    /// @return Returns the index of the item that where check returns true
    template <class Collection, class UnaryPred>
    std::optional<size_t> constexpr IndexOfIf(Collection const& collection, UnaryPred check) {
        auto i = std::find_if(collection.begin(), collection.end(), [&](auto& item) {
            return check(item);
        });

        GUARDR(i != collection.end(), std::nullopt);
        return std::distance(collection.begin(), i);
    }

    /// @return Returns the iterator for the first item that matches the check
    template <class Collection, class UnaryPred>
    constexpr typename Collection::const_iterator
    FirstIterator(Collection const& collection, UnaryPred check) {
        return std::find_if(collection.cbegin(), collection.cend(), check);
    }

    /// @return Returns the iterator for the first item that matches the check
    template <class Collection, class UnaryPred>
    constexpr typename Collection::iterator FirstIterator(Collection& collection, UnaryPred check) {
        return std::find_if(collection.begin(), collection.end(), check);
    }

    /// @return Returns the first item in the collection, if any
    template <class Collection>
    constexpr typename Collection::value_type SafeFirst(Collection& collection) {
        typename Collection::value_type defaultResult{};
        return !IsEmpty(collection) ? *collection.begin() : defaultResult;
    }

    /// @return Returns the first item that matches the check
    template <class Collection, class UnaryPred>
    constexpr std::optional<typename Collection::value_type>
    FirstIf(Collection const& collection, UnaryPred check) {
        auto i = FirstIterator(collection, check);
        return i != collection.end() ? *i : std::optional<typename Collection::value_type>();
    }

    /// @return Returns the first item that matches the check
    template <class Collection, class UnaryPred>
    constexpr std::optional<typename Collection::value_type>
    FirstIf(Collection& collection, UnaryPred check) {
        auto i = FirstIterator(collection, check);
        return i != collection.end() ? *i : std::optional<typename Collection::value_type>();
    }

    /// Filters items from the collection. If check returns true, the item is kept
    /// @return Returns the filtered collection
    template <class Collection, class UnaryPred>
    constexpr Collection Filter(Collection const& collection, UnaryPred check) {
        Collection result;
        std::copy_if(begin(collection), end(collection), std::back_inserter(result), check);
        return result;
    }

    /// Filters items from the collection. If check returns true, the item is kept
    /// @return Returns the filtered collection
    template <class Type, class UnaryPred>
    constexpr UnorderedSet<Type> Filter(UnorderedSet<Type> const& collection, UnaryPred check) {
        UnorderedSet<Type> result;
        std::copy_if(
            begin(collection), end(collection), std::inserter(result, result.begin()), check
        );
        return result;
    }

    /// Filters items from the collection. If check returns true, the item is kept
    /// @return Returns the filtered collection
    template <class Type, class UnaryPred>
    constexpr OrderedSet<Type> Filter(OrderedSet<Type> const& collection, UnaryPred check) {
        OrderedSet<Type> result;
        std::copy_if(
            begin(collection), end(collection), std::inserter(result, result.begin()), check
        );
        return result;
    }

    // MARK: Map

    /// Maps a collection of one type to a collection of a different type
    template <class ResultType, class Type, class Converter>
    constexpr VectorList<ResultType> Map(VectorList<Type> const& collection, Converter convert) {
        VectorList<ResultType> result;
        std::transform(begin(collection), end(collection), std::back_inserter(result), convert);
        return result;
    }

    /// Maps a collection of one type to a collection of a different type and eliminates
    /// nullopt/nullptr results
    template <class ResultType, class Type, class Converter>
    constexpr VectorList<ResultType>
    CompactMap(VectorList<Type> const& collection, Converter convert) {
        VectorList<ResultType> result;
        std::transform(begin(collection), end(collection), std::back_inserter(result), convert);
        Compact(result);
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

    /// Maps a collection of one type to a collection of a different type
    template <class ResultType, class Type, class Converter>
    constexpr OrderedSet<ResultType> Map(OrderedSet<Type> const& collection, Converter convert) {
        OrderedSet<ResultType> result;
        std::transform(
            begin(collection), end(collection), std::inserter(result, result.begin()), convert
        );
        return result;
    }

    /// @return Returns true if the index is valid for the collection
    template <class Collection>
    constexpr bool IsValidIndex(Collection& collection, size_t index) {
        return index >= 0 && index < collection.size();
    }

    /// MARK: Erase/Remove
    /// Removes the item at the specified index from the collection
    template <class Collection>
    constexpr void EraseAt(Collection& collection, std::size_t index) {
        GUARD(IsValidIndex(collection, index))
        collection.erase(collection.begin() + index);
    }

    /// Removes the item at the specified index from the collection
    template <class Collection>
    constexpr void RemoveAt(Collection& collection, std::size_t index) {
        EraseAt(collection, index);
    }

    /**
     Collects a tree in BFS order.

     Assumptions: Children() must be implemented, and nodes are pointers
     */
    template <class Node, class NodeList>
    void CollectBreadthFirstTree(Node* fromNode, NodeList& result) {
        result.clear();
        GUARD(fromNode)

        result.push_back(fromNode);

        std::function<void(NodeList&, Node*)> collectFunc = [&](NodeList& result, Node* fromNode) {
            for (auto& child : fromNode->Children()) {
                result.push_back(child.get());
            }
            for (auto& child : fromNode->Children()) {
                collectFunc(result, child.get());
            }
        };

        collectFunc(result, fromNode);
    }

    /**
     Collects a tree in BFS order.

     Assumptions: Children() must be implemented, and nodes are pointers
     */
    template <class Node, class NodeList, class UnaryPred>
    void CollectBreadthFirstTreeIf(Node* fromNode, NodeList& result, UnaryPred check) {
        result.clear();
        GUARD(fromNode)
        GUARD(check(fromNode))

        result.push_back(fromNode);

        std::function<void(NodeList&, Node*)> collectFunc = [&](NodeList& result, Node* fromNode) {
            for (auto& child : fromNode->Children()) {
                GUARD_CONTINUE(check(child.get()))
                result.push_back(child.get());
            }
            for (auto& child : fromNode->Children()) {
                GUARD_CONTINUE(check(child.get()))
                collectFunc(result, child.get());
            }
        };

        collectFunc(result, fromNode);
    }

    /// Appends the item to the collection
    template <class Collection>
    constexpr void Append(Collection& collection, typename Collection::value_type value) {
        collection.push_back(value);
    }

    /// Appends the item to the collection
    template <class Collection>
    constexpr void Add(Collection& collection, typename Collection::value_type value) {
        collection.push_back(value);
    }
} // namespace PJ
