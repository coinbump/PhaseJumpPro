#ifndef PJSOMENORMALRANDOM_H
#define PJSOMENORMALRANDOM_H

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 12/29/22
 */
namespace PJ
{
    /// <summary>
    /// Returns a normalized random value between 0-1.0
    /// This exists so we can inject specific random numbers for unit tests
    /// </summary>
    class SomeRandom
    {
    public:
        /// <summary>
        /// Value between 0-1.0
        /// </summary>
        virtual float Value() = 0;

        float VaryFloat(float value, float vary)
        {
            if (0 == vary) { return value; }
            return value + Delta(vary);
        }

        /// <summary>
        /// Returns a random value between -delta...delta.
        /// </summary>
        float Delta(float delta)
        {
            bool isVaryForward = Value() < 0.5f;
            float factor = isVaryForward ? 1.0f : -1.0f;
            return Value() * factor * delta;
        }
    };
}

#endif
