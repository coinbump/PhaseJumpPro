#ifndef PJSOMERANDOMCHOICE_H
#define PJSOMERANDOMCHOICE_H

#include "SomeRandom.h"
#include <math.h>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 2/18/23
 */
namespace PJ
{
    /// <summary>
    /// Returns a random choice of the specified type
    /// </summary>
    template <class Type>
    class SomeRandomChoice
    {
    public:
        virtual Type Choose(SomeRandom & random) = 0;
    };

    /// <summary>
    /// Returns value based on min/max values
    /// </summary>
    class MinMaxIntRandomChoice : public SomeRandomChoice<int>
    {
    public:
        int min;
        int max;

        MinMaxIntRandomChoice(int min, int max) : min(min), max(max)
        {
        }

        int Choose(SomeRandom & random) override
        {
            return min + (int)round(((float)max - (float)min) * random.Value());
        }
    };

    /// <summary>
    /// Returns value based on min/max values
    /// </summary>
    class MinMaxFloatRandomChoice : public SomeRandomChoice<float>
    {
    public:
        float min;
        float max;

        MinMaxFloatRandomChoice(float min, float max) : min(min), max(max)
        {
        }

        float Choose(SomeRandom & random) override
        {
            return min + ((max - min) * random.Value());
        }
    };

    /// <summary>
    /// Returns value based on variance values
    /// </summary>
    class VaryIntRandomChoice : public SomeRandomChoice<int>
    {
    public:
        int value;
        int vary;

        VaryIntRandomChoice(int value, int vary) : value(value), vary(vary)
        {
        }

        int Choose(SomeRandom & random) override
        {
            float floatValue = random.VaryFloat((float)value, (float)vary);
            return (int)round(floatValue);
        }
    };

    /// <summary>
    /// Returns value based on variance values
    /// </summary>
    class VaryFloatRandomChoice : public SomeRandomChoice<float>
    {
    public:
        float value;
        float vary;

        VaryFloatRandomChoice(float value, float vary) : value(value), vary(vary)
        {
        }

        float Choose(SomeRandom & random) override
        {
            return random.VaryFloat(value, vary);
        }
    };
}

#endif
