#ifndef PJWEIGHT_H
#define PJWEIGHT_H

#include "SomeTransform.h"
#include "Macros.h"

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 2/18/23
 */
namespace PJ
{
    /// <summary>
    /// Specifies the weight of a random event
    /// </summary>
    template <class Value>
    class Weight
    {
    protected:
        /// <summary>
        /// Base weight (never changes)
        /// </summary>
        float weight = 1.0f;

    public:
        using ValueTransform = SomeValueTransform<Weight<Value>>;

        Value value;

        /// <summary>
        /// Optional object that dynamically adjusts the weight during evaluation
        /// </summary>
        SP<ValueTransform> adjust;

        Weight(float weight) : weight(weight)
        {
        }

        Weight(float weight, Value value) : weight(weight), value(value)
        {
        }

        float WeightValue() const { return weight; }
        void SetWeightValue(float value) { weight = value; }
    };
}

#endif
