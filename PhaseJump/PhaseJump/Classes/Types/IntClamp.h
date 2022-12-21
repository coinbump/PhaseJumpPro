#ifndef PJINTCLAMP_H
#define PJINTCLAMP_H

#include "SomeTransform.h"
#include <algorithm>

/*
 * RATING: 5 stars
 * Simple utility with unit tests
 * CODE REVIEW: 12/7/22
 */
namespace PJ
{
    /// <summary>
    /// Clamp a int
    /// </summary>
    class IntClamp : public SomeValueTransform<int>
    {
    public:
        int minValue;
        int maxValue;

        IntClamp(int minValue, int maxValue) : minValue(minValue), maxValue(maxValue)
        {
        }

        int Transform(int value) const override
        {
            return std::clamp(value, minValue, maxValue);
        }
    };
}

#endif
