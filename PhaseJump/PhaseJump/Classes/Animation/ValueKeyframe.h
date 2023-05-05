#ifndef PJKEYFRAME_H
#define PJKEYFRAME_H

#include "SomeKeyframe.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/17/22
 */
namespace PJ
{
    template <class T>
    class ValueKeyframe : public SomeKeyframe
    {
    public:
        T value;

        /// <summary>
        /// Produces the interpolator for the animation curve
        /// </summary>
        SP<SomeKeyframeInterpolatorFactory<T>> interpolatorFactory;
    };
}

#endif
