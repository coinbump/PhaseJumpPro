using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Simple utility
CODE REVIEW: 12/29/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Returns a normalized random value between 0-1.0
    /// This exists so we can inject specific random numbers for unit tests
    /// </summary>
    public interface SomeNormalRandom
    {
        /// <summary>
        /// Value between 0-1.0
        /// </summary>
        public float Value { get; }
    }
}
