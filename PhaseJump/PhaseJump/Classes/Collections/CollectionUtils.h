#pragma once

#include "InfixOStreamIterator.h"
#include "SomeDataContainer.h"
#include "StringUtils.h"
#include "Utils.h"
#include <functional>
#include <optional>
#include <sstream>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/7/24
 */
namespace PJ {
    template <class Collection>
    void RemoveAll(Collection& collection) {
        collection.clear();
    }

    template <class Collection>
    Collection::size_type Count(Collection const& collection) {
        return collection.size();
    }

    template <class Collection>
    bool IsEmpty(Collection const& collection) {
        return collection.size() <= 0;
    }

    template <class Collection>
    void Remove(Collection& collection, typename Collection::value_type const& value) {
        auto i = std::find(collection.begin(), collection.end(), value);
        GUARD(i != collection.end())

        collection.erase(i);
    }

    template <class Collection, class UnaryPred>
    bool RemoveFirstIf(Collection& collection, UnaryPred check) {
        GUARDR(check, false)

        auto i = std::find_if(collection.begin(), collection.end(), check);
        GUARDR(i != collection.end(), false)

        collection.erase(i);
        return true;
    }

    template <class Collection, class UnaryPred>
    void RemoveIf(Collection& collection, UnaryPred check) {
        GUARD(check)
        collection.erase(
            std::remove_if(collection.begin(), collection.end(), check), collection.end()
        );
    }

    template <class Collection>
    bool Contains(Collection const& collection, typename Collection::value_type const& value) {
        auto findResult = std::find(collection.begin(), collection.end(), value);
        return findResult != collection.end();
    }

    template <class Collection, class UnaryPred>
    bool ContainsWhere(Collection const& collection, UnaryPred check) {
        GUARDR(check, false)
        auto i = std::find_if(collection.begin(), collection.end(), check);
        return i != collection.end();
    }

    template <class Collection>
    void AddRange(Collection& collection, Collection const& source) {
        collection.insert(collection.end(), source.begin(), source.end());
    }

    //    template <class Collection,
    //        class _Type = Collection,
    //        std::enable_if_t<std::is_same<_Type, std::vector<typename
    //        Collection::value_type>>::value>* = nullptr>
    //    void AddRange(Collection& collection, Collection const& source) {
    //        collection.reserve(collection.size() + source.size());
    //        collection.insert(collection.end(), source.begin(), source.end());
    //    }

    template <class Collection>
    std::optional<size_t>
    IndexOf(Collection const& collection, typename Collection::value_type const& item) {
        auto i = std::find(collection.begin(), collection.end(), item);

        GUARDR(i != collection.end(), std::nullopt);
        return std::make_optional(std::distance(collection.begin(), i));
    }

    template <class Collection, class UnaryPred>
    Collection::iterator FirstIterator(Collection& collection, UnaryPred check) {
        auto i = std::find_if(collection.begin(), collection.end(), check);
        GUARDR(i != collection.end(), collection.end())

        return i;
    }

    template <class Collection, class UnaryPred>
    std::optional<typename Collection::value_type> First(Collection& collection, UnaryPred check) {
        auto i = FirstIterator(collection, check);
        return i != collection.end() ? *i : std::optional<typename Collection::value_type>();
    }

    template <class Collection, class UnaryPred>
    Collection Filter(Collection const& collection, UnaryPred check) {
        Collection result;
        GUARDR(check, result)
        std::copy_if(begin(collection), end(collection), std::back_inserter(result), check);
        return result;
    }

    template <class Collection>
    void EraseAt(Collection& collection, std::size_t index) {
        if (index < 0 || index >= collection.size()) {
            return;
        }

        collection.erase(collection.begin() + index);
    }

    template <class Collection>
    void RemoveAt(Collection& collection, std::size_t index) {
        EraseAt(collection, index);
    }

    /**
     Collects a tree in BFS order.

     Makes some assumptions. That Children() is implemented, and nodes are pointers
     */
    template <class Node, class NodeList>
    void CollectBreadthFirstTree(Node const& fromNode, NodeList& result) {
        result.clear();
        result.push_back(fromNode);

        std::function<void(NodeList&, Node const&)> collectFunc = [&](NodeList& result,
                                                                      Node const& fromNode) {
            for (auto& child : fromNode->Children()) {
                result.push_back(child);
            }
            for (auto& child : fromNode->Children()) {
                collectFunc(result, child);
            }
        };

        collectFunc(result, fromNode);
    }

    template <class Collection>
    void Append(Collection& collection, typename Collection::value_type value) {
        collection.push_back(value);
    }

    template <class Collection>
    void Add(Collection& collection, typename Collection::value_type value) {
        collection.push_back(value);
    }

    template <class Collection>
    bool IsValidIndex(Collection& collection, size_t index) {
        return index >= 0 && index < collection.size();
    }

    template <class Collection>
    CollectionData<typename Collection::value_type> ToCollectionData(Collection& collection) {
        CollectionData<typename Collection::value_type> result(
            collection.size(), (void*)collection.data()
        );
        return result;
    }
} // namespace PJ
