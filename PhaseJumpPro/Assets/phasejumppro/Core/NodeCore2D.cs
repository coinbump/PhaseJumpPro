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

        [SerializeField]
        [Tooltip("Rotation in degrees")]
        /// <summary>Rotation in degrees</summary>
        protected Angle rotation;

        /// <summary>
        /// If true, clip rotation values between 0-360
        /// </summary>
        protected bool clipRotation = true;

        // Normalized rotation value (0-1.0)
        public float RotationNormal
        {
            get => rotation.Degrees / 360.0f;
            set
            {
                rotation.Degrees = value * 360.0f;
            }
        }

        public Angle Rotation
        {
            get => rotation;
            set
            {
                var newAngle = value;
                if (clipRotation)
                {
                    newAngle.Clip();
                }

                if (rotation.Degrees == newAngle.Degrees) { return; }
                rotation = newAngle;

                // Try-catch is for the unit test
                try
                {
                    if (transform)
                    {
                        transform.localEulerAngles = new Vector3(0, 0, -rotation.Degrees);
                    }
                }
                catch (NullReferenceException ex)
                {
                    Debug.Log(ex.Message);
                }
            }
        }

        public virtual void OnTransformLimited() { }

#if UNITY_EDITOR
        protected override void OnValidate()
        {
            base.OnValidate();

            transform.localEulerAngles = new Vector3(0, 0, -Rotation.Degrees);
        }
#endif
    }
}
