#ifndef PJFLOAT_TRANSFORM_H_
#define PJFLOAT_TRANSFORM_H_

#include "SomeTransform.h"

/*
 * RATING: 5 stars
 * Simple utility with unit test
 * CODE REVIEW: 11/5/22
 */
namespace PJ
{
    class FloatTransform : public SomeValueTransform<float>
    {
    };

    class FloatMultiplyTransform : public FloatTransform
    {
    public:
        float factor;

        FloatMultiplyTransform() : factor(1.0f) {
        }

        FloatMultiplyTransform(float factor) : factor(factor) {
        }

        virtual float Transform(float value) const override
        {
            return value * factor;
        }
    };
}

#endif  // FLOAT_TRANSFORM_H_
