#ifndef PJSOMELIMITER_H
#define PJSOMELIMITER_H

#include "Core/Updatable.h"

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 11/26/22
 */
namespace PJ
{
    /// <summary>
    /// A limiter limits whether or not an event can occur
    /// Owner must send OnUpdate events for timers to work
    /// </summary>
    class SomeLimiter : public Updatable
    {
    protected:
        virtual void OnFire() = 0;

    public:
        bool IsFinished() const override { return false; }

        bool Fire()
        {
            auto result = CanFire();
            if (result)
            {
                OnFire();
            }
            return result;
        }

        virtual bool CanFire() = 0;
    };
}

#endif
