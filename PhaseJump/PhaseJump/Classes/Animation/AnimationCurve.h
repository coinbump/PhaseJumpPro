#ifndef PJANIMATIONCURVE_H_
#define PJANIMATIONCURVE_H_

#include "Types/FloatTransform.h"

/*
 RATING: 5 stars
 Simple class
 CODE REVIEW: 11/12/22
 */
namespace PJ
{
    /// <summary>
    /// Specifies the details of an animation curve from start to end values, with an interpolation transformer
    /// </summary>
    template <class T>
    class AnimationCurve
    {
    public:
        T start;
        T end;

        float duration;

        FloatTransform *transform = new FloatMultiplyTransform(1.0f);

        AnimationCurve(T const& start, T const& end) : start(start), end(end) {
        }
        virtual ~AnimationCurve {}
    };
}

#endif
