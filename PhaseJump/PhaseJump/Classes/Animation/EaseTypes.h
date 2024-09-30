#pragma once

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/21/24
 */
namespace PJ {
    /// For editors to pick from list of types
    namespace EaseType {
        auto constexpr Linear = "linear";
        auto constexpr InSquared = "in.squared";
        auto constexpr InCubed = "in.cubed";
        auto constexpr OutSquared = "out.squared";
        auto constexpr OutCubed = "out.cubed";
        auto constexpr OutElastic = "out.elastic";
        auto constexpr InBack = "inBack";
        auto constexpr OutBack = "outBack";
        auto constexpr InOutBack = "inOutBack";
    }; // namespace EaseType
} // namespace PJ
