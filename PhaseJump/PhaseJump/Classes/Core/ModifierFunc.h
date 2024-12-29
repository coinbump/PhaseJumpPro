#pragma once

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/29/24
 */
namespace PJ {
    /// Allows compositional modification of objects
    template <class Type>
    using ModifierFunc = std::function<void(Type&)>;

    /// Allows compositional copy modification of objects
    template <class Type>
    using ModifierCopyFunc = std::function<Type(Type const&)>;
} // namespace PJ
