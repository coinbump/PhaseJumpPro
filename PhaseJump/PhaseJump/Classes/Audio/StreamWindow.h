#pragma once

/*
 RATING: 5+ stars
 Simple types
 CODE REVIEW: 4/17/26
 */
namespace PJ {
    /// A time window into a stream, measured in seconds.
    class StreamWindow {
    public:
        /// Window start time, in seconds.
        float start = 0.0f;

        /// Window duration, in seconds.
        float duration = 0.0f;

        StreamWindow() = default;

        StreamWindow(float start, float duration) :
            start(start),
            duration(duration) {}

        bool operator==(StreamWindow const& rhs) const {
            return start == rhs.start && duration == rhs.duration;
        }

        bool operator!=(StreamWindow const& rhs) const {
            return !(*this == rhs);
        }
    };
} // namespace PJ
