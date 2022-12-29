using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple utility class for 2D games with Unit Tests
 * CODE REVIEW: 11/23/22
 */
namespace PJ
{
    /// <summary>
    /// Core 2D node code
    /// </summary>
    public class NodeCore2D : Node2D
    {
        [Header("NodeCore2D Properties")]

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

        public virtual void OnTransformLimited() { }
    }
}
