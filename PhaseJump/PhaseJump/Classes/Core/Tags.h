#pragma once

#include "_String.h"
#include "OrderedMap.h"
#include "UnorderedMap.h"
#include "UnorderedSet.h"
#include <any>
#include <optional>
#include <set>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/15/24
 */
namespace PJ {
    using Tag = std::any;

    template <class T>
    struct DefaultAllocator {
        T operator()() {
            return T();
        }
    };

    /// Holds custom attributes (tags) in a dictionary
    struct Tags {
        using Key = String;
        using MapValue = Tag;
        using Map = UnorderedMap<Key, MapValue>;

        Map map;

        virtual ~Tags() {}

        std::pair<Map::iterator, bool> Insert(Map::value_type&& __x) {
            return map.insert_or_assign(__x.first, __x.second);
        }

        std::pair<Map::iterator, bool> Add(Map::value_type&& __x) {
            return map.insert_or_assign(__x.first, __x.second);
        }

        std::pair<Map::iterator, bool> Insert(Key const key, MapValue value) {
            return map.insert_or_assign(key, value);
        }

        std::pair<Map::iterator, bool> Add(Key const key, MapValue value) {
            return Insert({ key, value });
        }

        /// Creates a value if it doesn't exist
        template <class T, class Allocator = DefaultAllocator<T>>
        T SafeValue(Key key, Allocator allocator = Allocator()) const {
            auto value = map.find(key);

            if (value != map.end()) {
                try {
                    auto castValue = std::any_cast<T>(value->second);
                    return castValue;
                } catch (...) {}
            }

            return allocator();
        }

        template <class T>
        std::optional<T> Value(Key key) const {
            auto value = map.find(key);
            if (value != map.end()) {
                try {
                    auto castValue = std::any_cast<T>(value->second);
                    return std::make_optional<T>(castValue);
                } catch (...) {}
            }

            return std::nullopt;
        }

        /// Returns true if the value exists, and is of the correct type
        template <class T>
        bool ContainsTypeValue(Key key) const {
            auto value = map.find(key);
            if (value != map.end()) {
                try {
                    std::any_cast<T>(value->second);
                    return true;
                } catch (...) {}

                return false;
            }

            return false;
        }
    };

    /// Holds a set of type tag strings
    /// Example: "enemy", "ghost" tags for an arcade game
    class TypeTagsSet : UnorderedSet<String> {
    public:
        TypeTagsSet() :
            UnorderedSet<String>() {}

        TypeTagsSet(UnorderedSet<String> collection) :
            UnorderedSet<String>(collection) {}
    };
} // namespace PJ
