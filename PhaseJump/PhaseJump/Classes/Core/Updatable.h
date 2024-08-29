#pragma once

#include "TimeSlice.h"

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 6/13/24
 PORTED TO: C++, C#
 */
namespace PJ {
    /// An object that can receive time update events
    class Updatable {
    public:
        virtual ~Updatable() {}

        /// Called for each time delta event
        virtual void OnUpdate(TimeSlice time) = 0;

        /// Allows for cleanup of old updatables
        virtual bool IsFinished() const {
            return false;
        }
    };
} // namespace PJ
