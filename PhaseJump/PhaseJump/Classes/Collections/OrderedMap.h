#pragma once

#include "MapUtils.h"
#include "Utils.h"
#include <functional>
#include <map>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/3/24
 */
namespace PJ {
    /// Alias for std so can build an alternate implementation if needed
    template <class Key, class Type>
    using OrderedMap = std::map<Key, Type>;

    template <
        class Map, class Type,
        std::enable_if_t<std::is_same<std::map<typename Map::key_type, Type>, Map>::value>* =
            nullptr>
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
