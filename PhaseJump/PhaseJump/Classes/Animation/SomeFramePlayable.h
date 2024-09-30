#pragma once

#include "Playable.h"

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 9/25/24
 */
namespace PJ {
    /// Animates a frame value (for frame animations)
    class SomeFramePlayable : public Playable {
    public:
        SomeFramePlayable() {}

        virtual ~SomeFramePlayable() {}

        virtual int Frame() const = 0;

        int FrameForUpdate(TimeSlice time) {
            OnUpdate(time);
            return Frame();
        }
    };
} // namespace PJ
