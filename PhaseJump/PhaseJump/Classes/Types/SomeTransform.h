#ifndef PJSOME_TRANSFORM_H_
#define PJSOME_TRANSFORM_H_

#include "SomeMap.h"

/*
 RATING: 5 stars
 Simple class
 CODE REVIEW: 11/1/22
 */
namespace PJ {
    /// <summary>
    /// A class that transforms a value
    /// </summary>
    template <class Value, class Result>
    class SomeTransform
    {
    public:
        virtual Result Transform(Value value) const = 0;
    };

    template <class Value>
    class SomeValueTransform : public SomeTransform<Value, Value>
    {
    };

    template <class Value>
    class IdentityTransform : public SomeValueTransform<Value>
    {
    public:
        Value Transform(Value value) const override
        {
            return value;
        }
    };
}

#endif
