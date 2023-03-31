#ifndef PJSOMEANIMATEDVALUE_H
#define PJSOMEANIMATEDVALUE_H

#include "SomeTransform.h"

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 12/31/22
 */
namespace PJ
{
    /// <summary>
    /// An animated value changes from 0-1.0 normalized progress
    /// </summary>
    template <class T> class SomeAnimatedValue : public SomeTransform<float, T>
    {
    public:
        T Transform(float key) const override { return ValueAt(key); }

        virtual T ValueAt(float progress) const = 0;
    };
}

#endif
