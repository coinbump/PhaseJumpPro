using System;
using Godot;

/*
RATING: 5 stars
Simple component
CODE REVIEW: 12/18/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Base 2D world node
    /// </summary>
    public partial class NodeCore2D : WorldNode2D
    {
        protected PhysicsBody2D physicsBody2D;

        // Normalized rotation value (0-1.0)
        public float RotationNormal
        {
            get => ClippedRotationAngle.Degrees / 360.0f;
            set
            {
                ClippedRotationAngle = Angle.DegreesAngle(360.0f * value);
            }
        }

        public Angle ClippedRotationAngle
        {
            get => Angle.DegreesAngle(RotationDegrees).Clipped();
            set
            {
                var newAngle = value;
                newAngle.Clip();

                if (ClippedRotationAngle.Degrees == newAngle.Degrees) { return; }

                RotationDegrees = newAngle.Degrees;
            }
        }

        public bool IsKinematic
        {
            get => null == physicsBody2D || physicsBody2D is CharacterBody2D;
        }

        protected override void Awake()
        {
            base.Awake();

            // Some 2D objects use rigidbody for movement and physics
            if (GetParent() is PhysicsBody2D physicsBody2D)
            {
                this.physicsBody2D = physicsBody2D;
            }
        }

        // TODO:
        // /// <summary>
        // /// Move this node to the position in 2D space
        // /// (NOTE: This uses Vector3, but Rigidbody2D ignores the z component)
        // /// </summary>
        // public override void MoveToPosition(Vector3 position, bool force = false)
        // {
        //     if (IsKinematic || force)
        //     {
        //         transform.position = position;
        //     }
        //     else if (rigidbody)
        //     {
        //         rigidbody.MovePosition(position);
        //     }
        // }
    }
}
