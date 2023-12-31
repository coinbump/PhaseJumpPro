using Godot;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works
 * GODOT CODE REVIEW: 12/30/23
 */
namespace PJ
{
    /// <summary>
    /// Modulates object position (for kinematic objects)
    /// Useful for bouncing arrows, pointing at something to draw your attention
    /// IMPORTANT: this requires that the object be parented
    /// </summary>
    public partial class AnimateSinOffsetEffect : SomeEffect
    {
        [Export]
        public float cycleTime = 1.0f;
        [Export]
        public float scale = 1.0f;
        [Export]
        public Axis axis = Axis.Y;

        protected float timeValue = 0.0f;

        public override void Start()
        {
            // The target needs a parent object as a point of reference for the offset
            if (null == Target || null == Target.GetParent())
            {
                GD.Print("Error. Sin animation target requires parent");
            }
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (!IsOn) { return; }
            if (null == Target || null == Target.GetParent()) { return; }

            timeValue += time.delta;

            float offset = Mathf.Sin(timeValue * 2 * Mathf.Pi / cycleTime) * scale;

            if (Target is Node2D node2D)
            {
                var localPosition = node2D.Position;

                switch (axis)
                {
                    case Axis.X:
                        node2D.Position = new(offset, localPosition.Y);
                        break;
                    case Axis.Y:
                        node2D.Position = new(localPosition.X, offset);
                        break;
                    case Axis.Z:
                        node2D.Position = new(localPosition.X, localPosition.Y);
                        break;
                }
            }
            else if (Target is Node3D node3D)
            {
                var localPosition = node3D.Position;

                switch (axis)
                {
                    case Axis.X:
                        node3D.Position = new(offset, localPosition.Y, localPosition.Z);
                        break;
                    case Axis.Y:
                        node3D.Position = new(localPosition.X, offset, localPosition.Z);
                        break;
                    case Axis.Z:
                        node3D.Position = new(localPosition.X, localPosition.Y, offset);
                        break;
                }
            }
        }
    }
}
