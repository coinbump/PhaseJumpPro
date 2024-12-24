#pragma once

#include "UnorderedMap.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    /// Stores objects mapped by two keys: type and id
    template <class Type>
    class Catalog {
    public:
        /// First key: type, second key: id
        using CatalogMap = UnorderedMap<String, UnorderedMap<String, Type>>;

    protected:
        CatalogMap map;

    public:
        // FUTURE: add list if needed for entire catalog list

        CatalogMap const& Map() const {
            return map;
        }

        /// @return Returns the item it if it exists
        std::optional<Type> Find(String type, String id) const {
            try {
                auto& first = map.at(type);
                auto& second = first.at(id);
                return second;
            } catch (...) {
                return {};
            }
        }

        /// Adds a new item
        void Add(String type, String id, Type value) {
            map[type].insert_or_assign(id, value);
        }

        /// Removes an item
        void Remove(String type, String id) {
            auto i = map.find(type);
            GUARD(i != map.end())

            auto& secondMap = i->second;
            auto j = secondMap.find(id);
            GUARD(j != secondMap.end())

            secondMap.erase(j);
        }
    };
} // namespace PJ
