using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars. Simple utility class for 2D games with Unit Tests
 * CODE REVIEW: 4.8.18
 */
namespace PJ
{
	/// <summary>
	/// Provides utility methods for simplifying common 2D game scenarios
	/// </summary>
	public class Node2D : Node
	{
        [Header("Node2D Properties")]

		protected float _rotation;    // Simplifies basic 2D rotation

		public bool IsKinematic
        {
			get
            {
				return rb == null || rb.isKinematic;
            }
        }

		public bool dontModRotation = false;

		protected Rigidbody2D rb;

		// Normalized rotation value (0-1.0)
		public float RotationNormal
		{
			get { return _rotation; }
			set
			{
				if (!_rotation.Equals(value))
				{
					_rotation = value;
					if (!dontModRotation)
					{
						_rotation = value % 1.0f;
					}

					// Try-catch is for the unit test
					try
					{
						if (transform)
						{
							transform.eulerAngles = new Vector3(0, 0, -360.0f * _rotation);
						}
					}
					catch (NullReferenceException ex)
					{
						Debug.Log(ex.Message);
					}
				}
			}
		}

		public float RotationDegreeAngle
		{
			get { return _rotation * 360.0f; }
			set
			{
				RotationNormal = value / 360.0f;
			}
		}

		protected override void Start()
		{
			base.Start();

			rb = GetComponent<Rigidbody2D>();
		}

		public void MoveToPosition(Vector2 position, bool force = false)
		{
			var isKinematic = IsKinematic;

			if (isKinematic || force)
			{
				transform.position = position;
			}
			else
			{
				if (null != rb)
				{
					rb.MovePosition(position);
				}
			}
		}
	}
}