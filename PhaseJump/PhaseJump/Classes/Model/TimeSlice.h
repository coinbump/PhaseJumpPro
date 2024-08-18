#pragma once

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 6/8/24
 PORTED TO: C++, C#
 */
namespace PJ {
    /// Encapsulates time delta for update events
    /// Allows us to add additional metadata for time events if needed
    struct TimeSlice {
        float delta;

        TimeSlice(float delta) :
            delta(delta) {}

        TimeSlice operator*(float value) const {
            return TimeSlice{ delta * value };
        }
    };
} // namespace PJ
