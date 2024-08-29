#pragma once

#include "SomeLimiter.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/13/23
 */
// TODO: rethink this. Is this approach any better?
namespace PJ {
    /// Limits an action until N seconds have passed after the last time it
    /// happened Example: bullets only fire at a certain rate, rapid fire power
    /// up changes that
    namespace RateLimiter {
        struct Core {
            /// First fire isn't rate-limited
            bool didFire = false;

            /// Subsequent fires must have min delta between them
            float minDelta = 1.0f;

            /// Internal timer to track logic
            float timer = 0;

            Core() {}

            Core(float minDelta) :
                minDelta(minDelta) {}
        };

        using Type = SomeLimiter<Core>;

        SP<Type> Make();
    } // namespace RateLimiter
} // namespace PJ
