using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

/*
 * RATING: 5 stars. Simple utility class for 2D games with Unit Tests
 * CODE REVIEW: 4.8.18
 */
namespace PJ
{
	/// <summary>
	/// Provides utility methods for simplifying common 2D game scenarios
	/// </summary>
	public class Node2D : MonoBehaviour
	{
		private float _rotation;    // Simplifies basic 2D rotation
		private bool dontModRotation;

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

		public float RotationAngle
		{
			get { return _rotation * 360.0f; }
			set
			{
				RotationNormal = value / 360.0f;
			}
		}

		private void OnValidate()
		{
			transform.eulerAngles = new Vector3(0, 0, -360.0f * _rotation);
		}

		// Use this for initialization
		void Start()
		{

		}

		// Update is called once per frame
		void Update()
		{

		}

		public class UnitTests_Node2D {
			[Test]
			public void UnitTests() {
				var test = new Node2D();
				test.RotationAngle = 180.0f;
				Assert.AreEqual(test.RotationNormal, .5f);
				test.RotationAngle += 360.0f;
				Assert.AreEqual(test.RotationNormal, .5f);
			}
		}
	}
}
