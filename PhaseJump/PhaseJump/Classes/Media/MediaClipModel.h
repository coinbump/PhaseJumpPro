#pragma once

#include "StringUtils.h"
#include <optional>

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 4/21/26
 */
namespace PJ {
    /// Model for a clip on a media track.
    class MediaClipModel {
    public:
        using This = MediaClipModel;

        /// Clip start time on the track, in seconds.
        float start{};

        /// Clip duration, in seconds.
        float duration{};

        /// Optional identifier for lookups.
        String id;

        /// Offset within the source media, in seconds. When the clip is split, the right side
        /// advances this offset.
        float sourceStart{};

        MediaClipModel() = default;

        MediaClipModel(float start, float duration) :
            start(start),
            duration(duration) {}

        /// @return Returns the clip end time on the track.
        float End() const {
            return start + duration;
        }

        bool operator==(MediaClipModel const& rhs) const = default;
    };
} // namespace PJ
