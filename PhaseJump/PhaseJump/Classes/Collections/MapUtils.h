#pragma once

#include "CollectionUtils.h"
#include <map>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/14/25
 */
namespace PJ {
    template <class Map>
    concept IsStdMap =
        std::is_same_v<Map, std::map<typename Map::key_type, typename Map::mapped_type>>;

    template <class Map>
    concept IsStdUnorderedMap =
        std::is_same_v<Map, std::unordered_map<typename Map::key_type, typename Map::mapped_type>>;

    template <class Map>
        requires IsStdMap<Map>
    bool ContainsKey(Map const& map, typename Map::key_type const& key) {
        return map.find(key) != map.end();
    }

    template <class Map>
        requires IsStdUnorderedMap<Map>
    bool ContainsKey(Map const& map, typename Map::key_type const& key) {
        return map.find(key) != map.end();
    }

    template <class Map>
        requires IsStdMap<Map>
    bool ContainsIf(
        Map const& map,
        std::function<bool(std::pair<typename Map::key_type, typename Map::mapped_type> const&)>
            check
    ) {
        GUARDR(check, false)

        auto i = std::find_if(map.begin(), map.end(), check);
        return i != map.end();
    }

    template <class Map>
        requires IsStdUnorderedMap<Map>
    bool ContainsIf(
        Map const& map,
        std::function<bool(std::pair<typename Map::key_type, typename Map::mapped_type> const&)>
            check
    ) {
        GUARDR(check, false)

        auto i = std::find_if(map.begin(), map.end(), check);
        return i != map.end();
    }
} // namespace PJ
