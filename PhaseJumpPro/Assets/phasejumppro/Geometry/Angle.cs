using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple struct, with unit tests
 * CODE REVIEW: 6/21/22
 */
namespace PJ
{
    public struct Angle
    {
        private float value;

        public float Degrees
        {
            get => value;
            set
            {
                this.value = value;
            }
        }

        public float Radians
        {
            get => value * Mathf.Deg2Rad;
            set
            {
                this.value = value / Mathf.Deg2Rad;
            }
        }

        private Angle(float value)
        {
            this.value = value;
        }

        public static Angle RadiansAngle(float radians)
        {
            return new Angle(radians / Mathf.Deg2Rad);
        }

        public static Angle DegreesAngle(float degrees)
        {
            return new Angle(degrees);
        }

        public static Angle zero = DegreesAngle(0);
    }
}
