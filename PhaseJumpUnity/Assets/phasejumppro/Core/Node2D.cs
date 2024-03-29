using System;
using UnityEngine;

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
    public class Node2D : SomeNode
    {
        /// <summary>
        /// If true, clip rotation values between 0-360
        /// </summary>
        protected bool clipRotation = true;

        // Normalized rotation value (0-1.0)
        public float RotationNormal
        {
            get => Rotation.Degrees / 360.0f;
            set
            {
                Rotation = Angle.DegreesAngle(360.0f * value);
            }
        }

        public Angle Rotation
        {
            get => Angle.DegreesAngle(-transform.localEulerAngles.z).Clipped();
            set
            {
                var newAngle = value;
                if (clipRotation)
                {
                    newAngle.Clip();
                }

                if (Rotation.Degrees == newAngle.Degrees) { return; }

                // Try-catch is for the unit test
                try
                {
                    transform.localEulerAngles = new Vector3(0, 0, -newAngle.Degrees);
                }
                catch (NullReferenceException ex)
                {
                    Debug.Log(ex.Message);
                }
            }
        }

        public override bool IsKinematic
        {
            get => null == rigidbody ? true : rigidbody.isKinematic;
        }

        protected new Rigidbody2D rigidbody;

        protected override void Awake()
        {
            base.Awake();

            // Some 2D objects use rigidbody for movement and physics
            if (TryGetComponent(out Rigidbody2D rigidbody))
            {
                this.rigidbody = rigidbody;
            }
        }

        /// <summary>
        /// Move this node to the position in 2D space
        /// (NOTE: This uses Vector3, but Rigidbody2D ignores the z component)
        /// </summary>
        public override void MoveToPosition(Vector3 position, bool force = false)
        {
            if (IsKinematic || force)
            {
                transform.position = position;
            }
            else if (rigidbody)
            {
                rigidbody.MovePosition(position);
            }
        }
    }
}
