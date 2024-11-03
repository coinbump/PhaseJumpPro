#pragma once

#include <list>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/20/24
 */
namespace PJ {
    /// Alias for std so can build an alternate implementation if needed
    template <class T, class Allocator = std::allocator<T>>
    using List = std::list<T, Allocator>;
} // namespace PJ
