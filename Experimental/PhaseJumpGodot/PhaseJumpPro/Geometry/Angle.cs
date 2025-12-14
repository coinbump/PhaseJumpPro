using System;
using Godot;

// TODO: Unit tests
namespace PJ
{
    [Serializable]
    public struct Angle : IComparable<float>, IEquatable<float>
    {
        [Export(PropertyHint.Range, "0,360,")]
        private float value;

        public float Degrees
        {
            readonly get => value;
            set
            {
                this.value = value;
            }
        }

        public float Radians
        {
            readonly get => Mathf.DegToRad(value);
            set
            {
                this.value = Mathf.RadToDeg(value);
            }
        }

        private Angle(float value)
        {
            this.value = value;
        }

        public Angle(Vector2 distance)
        {
            // Prevent infinite/invalid angle
            if (distance.X == 0 && distance.Y == 0)
            {
                this.value = 0;
                return;
            }

            float radians = Mathf.Atan2(distance.Y * Vector2.Down.Y, distance.X);
            var angle = Angle.DegreesAngle(Mathf.RadToDeg(radians) + 90.0f);
            var result = angle.Clipped();
            this.value = result.Degrees;
        }

        public Angle Clipped()
        {
            var angle = Degrees;
            angle %= 360.0f;
            if (angle < 0)
            {
                angle = 360.0f - (Mathf.Abs(angle) % 360.0f);
            }
            return Angle.DegreesAngle(angle);
        }

        public void Clip()
        {
            Degrees = Clipped().Degrees;
        }

        public static Angle RadiansAngle(float radians)
        {
            return new Angle(Mathf.RadToDeg(radians));
        }

        public static Angle DegreesAngle(float degrees)
        {
            return new Angle(degrees);
        }

        public readonly int CompareTo(float other)
        {
            return value.CompareTo(other);
        }

        public readonly bool Equals(float other)
        {
            return value == other;
        }

        public readonly Vector2 ToVector2(float magnitude = 1.0f)
        {
            Vector2 result = new(0, 0);

            float sinVal = Mathf.Sin(Radians);
            float cosVal = Mathf.Cos(Radians);

            float x = magnitude * sinVal;
            float y = magnitude * cosVal * Vector2.Up.Y;
            result.X = x;
            result.Y = y;

            return result;
        }

        /// <summary>
        /// Returns the closest turn between two angles.
        /// EXAMPLE: closest turn between angle 3 and angle 359 is -4, not 356.
        /// </summary>
        public readonly Angle MinAngleTo(Angle angle)
        {
            float firstAngle = Degrees;
            float finalAngle = angle.Degrees;

            float delta1, delta2;
            float result = 0;

            if (finalAngle > firstAngle)
            {
                delta1 = finalAngle - firstAngle;
                delta2 = -(360.0f - delta1);
            }
            else
            {
                delta1 = -(firstAngle - finalAngle);
                delta2 = 360.0f + delta1;
            }

            if (Mathf.Abs(delta1) < Mathf.Abs(delta2))
            {
                result = delta1;
            }
            else
            {
                result = delta2;
            }

            return Angle.DegreesAngle(result);
        }

        public static implicit operator Vector2(Angle angle) => angle.ToVector2();

        public static Angle operator +(Angle left, Angle right)
        {
            return new Angle(left.value + right.value);
        }

        public static Angle operator -(Angle left, Angle right)
        {
            return new Angle(left.value - right.value);
        }

        public static Angle operator *(Angle left, Angle right)
        {
            return new Angle(left.value * right.value);
        }

        public static Angle operator /(Angle left, Angle right)
        {
            return new Angle(left.value / right.value);
        }

        public static Angle zero = DegreesAngle(0);
    }
}
