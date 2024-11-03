#pragma once

#include "SomeLimiter.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/5/24
 */
namespace PJ {
    struct RateLimiterCore {
        /// First fire isn't rate-limited
        bool didFire = false;

        /// Subsequent fires must have min delta between them
        float rate = 1.0f;

        /// Internal timer to track logic
        float timer = 0;

        RateLimiterCore() {}

        RateLimiterCore(float minDelta) :
            rate(minDelta) {}
    };

    /// Limits an action until N seconds have passed after the last time it
    /// happened Example: bullets only fire at a certain rate, rapid fire power
    /// up changes that
    class RateLimiter : public Limiter<RateLimiterCore> {
    public:
        using This = RateLimiter;

        RateLimiter(float minDelta);
    };
} // namespace PJ
