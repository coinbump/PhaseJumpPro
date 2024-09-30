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

        /// (Optional). Custom properties
        Tags tags;

        /// (Optional). Object attribute types (what kind of object is this?)
        TypeTagSet typeTags;

        virtual ~SomeKeyframe() {}

        MathType time{};
    };
} // namespace PJ
