#pragma once

#include "MapUtils.h"
#include "Utils.h"
#include <unordered_map>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/14/25
 */
namespace PJ {
    /// Alias for standard type so you can build an alternate implementation if needed
    template <class Key, class Type>
    using UnorderedMap = std::unordered_map<Key, Type>;
} // namespace PJ
