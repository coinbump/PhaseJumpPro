#ifndef PJFIXEDNORMALRANDOM_H
#define PJFIXEDNORMALRANDOM_H

#include "Array.h"

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 12/29/22
 */
namespace PJ
{
    /// <summary>
    /// Returns a fixed set of random values (for testing, debugging)
    /// </summary>
    class FixedRandom : public SomeRandom
    {
    public:
        Array<float> values;

    protected:
        int valueIndex = 0;

    public:
        float Value() override
        {
            if (values.Count() == 0) { return 0; }

            auto index = valueIndex % values.Count();
            valueIndex++;
            return values[index];
        }
    };
}

#endif
