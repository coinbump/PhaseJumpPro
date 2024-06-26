﻿using System;
using Godot;

/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 11/5/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Clamp a int
    /// </summary>
    public class IntClamp : SomeValueTransform<int>
    {
        public int minValue;
        public int maxValue;

        public IntClamp(int minValue, int maxValue)
        {
            this.minValue = minValue;
            this.maxValue = maxValue;
        }

        public override int Transform(int value)
        {
            return Math.Clamp(value, minValue, maxValue);
        }
    }
}
