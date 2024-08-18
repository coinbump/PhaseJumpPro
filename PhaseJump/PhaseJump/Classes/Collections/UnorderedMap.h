#pragma once

#include "MapUtils.h"
#include "Utils.h"
#include <unordered_map>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/3/24
 */
namespace PJ {
    template <class Key, class Type>
    using UnorderedMap = std::unordered_map<Key, Type>;

    template <
        class Map, class Type,
        std::enable_if_t<
            std::is_same<std::unordered_map<typename Map::key_type, Type>, Map>::value>* = nullptr>
    bool MapContainsWhere(Map const& map, std::function<bool(Type const&)> check) {
        GUARDR(check, false)

        for (auto& pair : map) {
            auto& value = pair.second;

            if (check(value)) {
                return true;
            }
        }

        return false;
    }
} // namespace PJ
