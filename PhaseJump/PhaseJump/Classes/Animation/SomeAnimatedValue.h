#ifndef PJSOMEANIMATEDVALUE_H
#define PJSOMEANIMATEDVALUE_H

#include "SomeTransform.h"

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 1/12/24
 */
namespace PJ
{
    /// <summary>
    /// An animated value changes from 0-1.0 normalized progress
    /// </summary>
    template <class T> class SomeAnimatedValue : public SomeTransform<float, T>
    {
    public:
        virtual ~SomeAnimatedValue() {}

        T ValueAt(float progress) const {
            return this->Transform(progress);
        }
    };
}

#endif
