#ifndef PJANIMATOR_H_
#define PJANIMATOR_H_

#include "TimeSlice.h"

/*
 * RATING: 5 stars
 * Simple interface
 * CODE REVIEW: 11/5/22
 */
namespace PJ
{
    /// <summary>
    /// An object that can receive time update events
    /// </summary>
    class Updatable
    {
    public:
        virtual ~Updatable() {}
        
        virtual void OnUpdate(TimeSlice time) = 0;
        virtual bool IsFinished() const = 0;
    };
}

#endif
