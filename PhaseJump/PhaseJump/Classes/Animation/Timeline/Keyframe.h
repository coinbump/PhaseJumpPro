#pragma once

#include "Tags.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/19/25
 */
namespace PJ {
    /// Keyframe at a specific point in time
    class Keyframe {
    public:
        using MathType = float;

        /// Custom properties
        Tags tags;

        /// Object attribute types
        TypeTagSet typeTags;

        virtual ~Keyframe() {}

        /// The position of this keyframe in time space
        MathType time{};
    };
} // namespace PJ
