
using Godot;

/*
 * RATING: 5 stars
 * Simple struct
 * CODE REVIEW: 4/21/22
 */
namespace PJ
{
    /// <summary>
    /// An angle with 2 components: angle (normalized) and magnitude
    /// </summary>
    public struct Angle2D
    {
        public float angle; // 0-1.0 (.5 is 180 degrees always pointing screen down)
        public float magnitude;

        public float DegreeAngle
        {
            readonly get => angle * 360.0f;
            set
            {
                angle = value / 360.0f;
            }
        }

        public Angle2D(float angle, float magnitude)
        {
            this.angle = angle;
            this.magnitude = magnitude;
        }

        public Angle2D(Vector2 vector)
        {
            float squaredMagnitude = vector.X * vector.X + vector.Y * vector.Y;
            magnitude = Mathf.Sqrt(squaredMagnitude);

            Vector2 vp = new(vector.X, vector.Y);
            angle = new Angle(vp).Degrees / 360.0f; // Normalize it
        }

        public readonly Vector2 ToVector2()
        {
            return Angle.DegreesAngle(this.angle * 360.0f).ToVector2(this.magnitude);
        }
    }
}
