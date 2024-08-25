#pragma once

#include "CollectionUtils.h"
#include <list>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/20/24
 */
namespace PJ {
    /// Aliases std::list so can build an alternate implementation later if needed
    template <class T, class Allocator = std::allocator<T>>
    using List = std::list<T, Allocator>;
} // namespace PJ
