using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple utility class for 2D games with Unit Tests
 * CODE REVIEW: 11.23.22
 */
namespace PJ
{
	/// <summary>
	/// Core 2D node code
	/// </summary>
	public class NodeCore2D : SomeNode
	{
		[Header("NodeCore2D Properties")]

		[SerializeField]
		[Range(0, 360.0f)]
        [Tooltip("Rotation in degrees")]
		/// <summary>Rotation in degrees</summary>
		protected float rotation = 0;

		public override bool IsKinematic
        {
			get
            {
				return rigidbody == null || rigidbody.isKinematic;
            }
        }

		/// <summary>
		/// If true, clip rotation values between 0-360
		/// </summary>
		protected bool clipRotation = true;

		protected new Rigidbody2D rigidbody;

		// Normalized rotation value (0-1.0)
		public float RotationNormal
		{
			get { return rotation / 360.0f; }
			set
			{
				rotation = value * 360.0f;
			}
		}

		public float Rotation
		{
			get { return rotation; }
			set
			{
				var newAngle = value;
				if (clipRotation)
				{
					newAngle = AngleUtils.ClipDegreeAngle(newAngle);
				}

				if (rotation == newAngle) { return; }
				rotation = newAngle;

				// Try-catch is for the unit test
				try
				{
					if (transform)
					{
						transform.localEulerAngles = new Vector3(0, 0, -rotation);
					}
				}
				catch (NullReferenceException ex)
				{
					Debug.Log(ex.Message);
				}
			}
		}

        protected override void Awake()
        {
            base.Awake();

			// Some 2D objects use rigidbody for movement and physics
			if (TryGetComponent(out Rigidbody2D rigidbody)) {
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
				//Debug.Log("Move to Position: Kinematic");

				transform.position = position;
			}
			else
			{
				if (null != rigidbody)
				{
					//Debug.Log("Move to Position: Physics");

					// MovePosition is for physics-based objects (non kinematic)
					rigidbody.MovePosition(position);
				}
			}
		}

		public virtual void OnTransformLimited() { }

#if UNITY_EDITOR
        protected override void OnValidate()
        {
            base.OnValidate();

			transform.localEulerAngles = new Vector3(0, 0, -Rotation);
		}
#endif
    }
}
