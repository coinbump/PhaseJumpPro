using Godot;
using System;
using System.Collections.Generic;
using System.Linq;

/*
 * RATING: 4 stars
 * Needs unit tests, better episolon default values?
 * CODE REVIEW: 4/8/22
 */
namespace PJ
{
    public static class Utils
    {
        // TODO: Unit tests
        public enum RaycastType
        {
            All, First
        }

        public const float FloatEpsilon = 1.192092896e-07F;

        public static bool Equals(float a, float b, float epsilon = FloatEpsilon)
        {
            return Mathf.Abs(Mathf.Abs(a) - Mathf.Abs(b)) < epsilon;
        }

        // FUTURE: need better epsilon default here
        public static bool Equals(double a, double b, double epsilon = .00001)
        {
            return Math.Abs(Math.Abs(a) - Math.Abs(b)) < epsilon;
        }
    }
}
