#pragma once

#include "Tags.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/21/24
 */
namespace PJ {
    /// Keyframe at a specific point in time
    class SomeKeyframe {
    public:
        using MathType = float;

        /// Custom properties
        Tags tags;

        /// Object attribute types
        TypeTagSet typeTags;

        virtual ~SomeKeyframe() {}

        /// The position of this keyframe in time space
        MathType time{};
    };
} // namespace PJ
