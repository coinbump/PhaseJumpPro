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
    template <class Value>
    class SomeTransform : public SomeMap<Value, Value>
    {
    public:
        virtual Value Transform(Value value) const = 0;

        virtual Value ValueFor(Value key) const
        {
            return Transform(key);
        }
    };
}

#endif  // SOME_TRANSFORM_H_
