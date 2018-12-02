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
		public bool isKinematic;

		// Paths:
		[System.Serializable]
		public class PathInfo
		{
			public GameObject path;
			public float speed = 1.0f;
			public int waypointOrigin;

			[UnityEngine.Range(0, 1.0f)]
			public float waypointProgress;

			public bool isMovingForward = true;
			public bool orientToPath = false;
		}
		public PathInfo pathInfo = new PathInfo();

		[Space(10)]
		// Kinematic motion:
		public float directionVelocity;
		public Vector2 velocity;

		public bool dontModRotation = false;

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

		protected virtual void OnValidate()
		{
			transform.eulerAngles = new Vector3(0, 0, -360.0f * _rotation);

			if (null == pathInfo.path) { return; }
			MovePath2D movePath = pathInfo.path.GetComponent("MovePath2D") as MovePath2D;
			if (null == movePath) { return; }

			pathInfo.waypointOrigin = Math.Min(pathInfo.waypointOrigin, movePath.waypoints.Count - 1);
			pathInfo.waypointOrigin = Math.Max(pathInfo.waypointOrigin, 0);
			SnapToPath();
		}

		// Update is called once per frame
		protected virtual void Update()
		{
			UpdateNode(false);
		}

		protected virtual void FixedUpdate()
		{
			UpdateNode(true);
		}

		protected void GetPathPositions(MovePath2D movePath, out Vector2 positionStart, out Vector2 positionEnd)
		{
			int origin = pathInfo.waypointOrigin;
			var positions = movePath.GetWaypointPositions2D();

			positionStart = transform.position;
			positionEnd = transform.position;

			if (positions.Count < 2) { return; }

			positionStart = positions[origin];
			positionEnd = positionStart;

			if (pathInfo.isMovingForward)
			{
				positionEnd = positions[origin + 1];
			}
			else
			{
				if (origin > 0)
				{
					positionEnd = positions[origin - 1];
				}
				else if (movePath.isLooping)
				{
					positionEnd = positions[positions.Count - 2];
				}
			}
		}

		protected void SnapToPath()
		{
			if (null == pathInfo.path) { return; }
			MovePath2D movePath = pathInfo.path.GetComponent("MovePath2D") as MovePath2D;
			if (null == movePath)
			{
				Debug.Log("Warning. Path is missing MovePath2D component"); return;
			}

			int origin = pathInfo.waypointOrigin;
			var positions = movePath.GetWaypointPositions2D();

			if (positions.Count < 2) { return; }

			Vector2 positionStart;
			Vector2 positionEnd;
			GetPathPositions(movePath, out positionStart, out positionEnd);

			Vector2 position = Vector2.Lerp(positionStart, positionEnd, pathInfo.waypointProgress);

			transform.position = position;

			if (pathInfo.orientToPath)
			{
				float angle = Mathf.Atan2(positionEnd.x - positionStart.x, positionEnd.y - positionStart.y) * Mathf.Rad2Deg;
				RotationAngle = angle;
			}
		}

		/// <summary>
		/// On update, moves the object along a MovePath2D.
		/// 
		/// FUTURE: add interpolation support if needed.
		/// FUTURE: support spline paths/curves
		/// 
		/// </summary>
		protected void UpdateFollowPath()
		{
			if (null == pathInfo.path) { return; }
			MovePath2D movePath = pathInfo.path.GetComponent("MovePath2D") as MovePath2D;
			if (null == movePath) {
				Debug.Log("Warning. Path is missing MovePath2D component"); return; 
			}

			var positions = movePath.GetWaypointPositions2D();

			// TODO: this only works with simple paths, we need better logic here.
			pathInfo.waypointProgress += Time.deltaTime*pathInfo.speed;

			while (pathInfo.waypointProgress >= 1.0f)
			{
				pathInfo.waypointProgress -= 1.0f;

				if (pathInfo.isMovingForward) {
					pathInfo.waypointOrigin += 1;

					if (movePath.isLooping)
					{
						if (pathInfo.waypointOrigin >= positions.Count - 1) {
							pathInfo.waypointOrigin = 0;
						}
					}
					else
					{
						if (pathInfo.waypointOrigin >= positions.Count - 1)
						{
							pathInfo.waypointOrigin = movePath.waypoints.Count - 1;
							pathInfo.isMovingForward = false;
						}
					}
				}
				else
				{
					pathInfo.waypointOrigin -= 1;
					if (pathInfo.waypointOrigin < 0 && movePath.isLooping)
					{
						pathInfo.waypointOrigin = movePath.waypoints.Count - 1;
					}
					else if (pathInfo.waypointOrigin <= 0 && !movePath.isLooping)
					{
						pathInfo.waypointOrigin = 0;
						pathInfo.isMovingForward = true;
					}
				}
			}

			SnapToPath();
		}

		protected void UpdateNode(bool isFixedUpdate)
		{
			// MOVEMENT TYPE: Follow Path (requires MovePath2D component on Path object)
			if (pathInfo.path != null)
			{
				UpdateFollowPath();
				return;
			}

			// MOVEMENT TYPE: Directional Velocity (for objects that move where they're pointed)
			var velocityVector = velocity;
			if (!directionVelocity.Equals(0))
			{
				var radians = RotationAngle * 0.01745329252f;
				velocityVector = new Vector2((float)Math.Sin(radians) * directionVelocity, (float)Math.Cos(radians) * directionVelocity);
			}

			// MOVEMENT TYPE: Kinematic velocity (for objects that don't require physics)
			var position = transform.position;
			position.x += velocityVector.x * Time.deltaTime;
			position.y += velocityVector.y * Time.deltaTime;

			if (isKinematic && !isFixedUpdate)
			{
				transform.position = position;
			}
			else if (!isKinematic && isFixedUpdate)
			{
				Rigidbody2D rigidbodyComponent = GetComponent<Rigidbody2D>();
				if (null != rigidbodyComponent) {
					rigidbodyComponent.MovePosition(position);
				}
			}
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
