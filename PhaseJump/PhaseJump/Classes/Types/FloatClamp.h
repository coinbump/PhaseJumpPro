#ifndef PJFLOATCLAMP_H_
#define PJFLOATCLAMP_H_

#include <algorithm>
#include "SomeTransform.h"

/*
 * RATING: 5 stars
 * Simple utility with unit tests
 * CODE REVIEW: 12/7/22
 */
namespace PJ
{
    /// <summary>
    /// Clamp a float
    /// </summary>
    class FloatClamp : public SomeValueTransform<float>
    {
    public:
        float minValue;
        float maxValue;

        FloatClamp(float minValue, float maxValue) : minValue(minValue), maxValue(maxValue)
        {
        }

        float Transform(float value) const override
        {
            return std::clamp(value, minValue, maxValue);
        }
    };
}

#endif
