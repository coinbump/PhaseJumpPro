#pragma once

#include <vector>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Alias for std so can build an alternate implementation if needed
    template <class T, class Allocator = std::allocator<T>>
    using VectorList = std::vector<T, Allocator>;

    // MARK: Convenience

    template <class T, class Allocator = std::allocator<T>>
    using VecList = VectorList<T, Allocator>;
} // namespace PJ
