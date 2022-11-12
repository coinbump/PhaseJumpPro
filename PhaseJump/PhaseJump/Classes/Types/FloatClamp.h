#ifndef PJFLOATCLAMP_H_
#define PJFLOATCLAMP_H_

#include <algorithm>
#include "SomeTransform.h"

/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 11/5/22
 */
namespace PJ
{
    /// <summary>
    /// Clamp a float
    /// </summary>
    class FloatClamp : public SomeTransform<float>
    {
    public:
        float minValue;
        float maxValue;

        FloatClamp(float minValue, float maxValue) : minValue(minValue), maxValue(maxValue)
        {
        }

        virtual float Transform(float value)
        {
            return std::clamp(value, minValue, maxValue);
        }
    };
}

#endif
