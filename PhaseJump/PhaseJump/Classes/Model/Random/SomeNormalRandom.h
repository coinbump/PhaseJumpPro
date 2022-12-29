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
    class SomeNormalRandom
    {
    public:
        /// <summary>
        /// Value between 0-1.0
        /// </summary>
        virtual float Value() = 0;
    };
}

#endif
