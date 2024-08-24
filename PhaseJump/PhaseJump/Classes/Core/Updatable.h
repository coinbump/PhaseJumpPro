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

    // TODO: unfinished
    //    class Updatables : public Updatable {
    //    public:
    //        VectorList<WP<Updatable>> updatables;
    //
    //        // MARK: Updatable
    //
    //        /// Called for each time delta event
    //        virtual void OnUpdate(TimeSlice time) {
    //            VectorList<WP<Updatable>> freshUpdatables;
    //
    //            std::for_each(updatables.begin(), updatables.end(), [](WP<Updatable> u) {
    //                GUARD(!u.expired())
    //
    //                auto lock = u.lock();
    //                freshUpdatables.Add(lock);
    //            });
    //
    //
    //        }
    //
    //        /// Allows for cleanup of old updatables
    //        virtual bool IsFinished() const {
    //            return false;
    //        }
    //    };
} // namespace PJ
