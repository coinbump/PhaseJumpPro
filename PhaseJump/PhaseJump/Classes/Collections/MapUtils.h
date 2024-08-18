#pragma once

#include "CollectionUtils.h"
#include <map>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/3/24
 */
namespace PJ {
    template <class Map, class Key>
    bool ContainsKey(Map const& map, Key const& key) {
        return map.find(key) != map.end();
    }
} // namespace PJ
