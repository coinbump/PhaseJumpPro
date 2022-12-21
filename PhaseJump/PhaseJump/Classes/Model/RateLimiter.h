#ifndef PJRATELIMITER_H
#define PJRATELIMITER_H

#include "SomeLimiter.h"

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 11/26/22
 */
namespace PJ
{
    /// <summary>
    /// Limits an action until N seconds have passed after the last time it happened
    /// Example: bulllets only fire at a certain rate, rapid fire power up changes that
    /// </summary>
    class RateLimiter : public SomeLimiter
    {
    protected:
        void OnFire() override
        {
            timer = 0;
        }

    public:
        float minDelta = 1.0f;
        float timer = 0;

        RateLimiter(float minDelta) : minDelta(minDelta)
        {
        }

        bool CanFire() override
        {
            return timer >= minDelta;
        }

        void OnUpdate(TimeSlice time) override
        {
            timer += time.delta;
        }
    };
}

#endif
