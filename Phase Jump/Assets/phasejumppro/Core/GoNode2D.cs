using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
	/// <summary>
	/// Extends the core Node2D class with more common 2D game scenarios, such as paths and movement.
	/// </summary>
	public class GoNode2D : Node2D
	{
		// Paths:
		[Serializable]
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
		public float rotationSpeed; // Normalized angles per second

		protected override void OnValidate()
		{
			base.OnValidate();

			transform.eulerAngles = new Vector3(0, 0, -360.0f * _rotation);

			if (null == pathInfo.path) { return; }
			SomeMovePath2D movePath = pathInfo.path.GetComponent<SomeMovePath2D>();
			if (null == movePath) { return; }

			if (!movePath.targets.Contains(gameObject))
			{
				movePath.targets.Add(gameObject);
			}

			SnapToPath(true);
		}

		public bool ShouldMoveForUpdate(UpdateType updateType)
		{
			bool isFixedUpdate = updateType == UpdateType.Fixed;

			var isKinematic = IsKinematic;

			if (isKinematic && !isFixedUpdate)
			{
				return true;
			}
			else if (!isKinematic && isFixedUpdate)
			{
				return true;
			}

			return false;
		}

		/// <summary>
		/// On update, moves the object along a MovePath2D.
		/// 
		/// FUTURE: add interpolation support if needed.
		/// FUTURE: support spline paths/curves
		/// 
		/// </summary>
		protected void UpdateFollowPath(UpdateType updateType)
		{
			if (!ShouldMoveForUpdate(updateType)) { return; }
			if (null == pathInfo.path) { return; }
			SomeMovePath2D movePath = pathInfo.path.GetComponent<SomeMovePath2D>();
			if (null == movePath)
			{
				Debug.Log("Warning. Path is missing Path2D component"); return;
			}

			movePath.UpdateNodeOnPath(this);
			SnapToPath();
		}

		protected override void UpdateNode(UpdateType updateType)
		{
			base.UpdateNode(updateType);

			if (Mathf.Abs(rotationSpeed) > 0)
			{
				RotationDegreeAngle += rotationSpeed * 360.0f * Time.deltaTime;
			}

			// MOVEMENT TYPE: Follow Path (requires MovePath2D component on Path object)
			if (pathInfo.path != null)
			{
				UpdateFollowPath(updateType);
				return;
			}

			// MOVEMENT TYPE: Directional Velocity (for objects that move where they're pointed)
			var velocityVector = velocity;
			if (!directionVelocity.Equals(0))
			{
				velocityVector = AngleUtils.DegreeAngleToVector2(RotationDegreeAngle, directionVelocity);
			}
			if (velocityVector == new Vector2(0, 0)) { return; }

			// MOVEMENT TYPE: Kinematic velocity (for objects that don't require physics)
			var position = transform.position;
			position.x += velocityVector.x * Time.deltaTime;
			position.y += velocityVector.y * Time.deltaTime;

			if (ShouldMoveForUpdate(updateType))
			{
				MoveToPosition(position);
			}
		}

		public void SnapToPath(bool force = false)
		{
			if (null == pathInfo.path) { return; }
			SomeMovePath2D movePath = pathInfo.path.GetComponent<SomeMovePath2D>();
			if (null == movePath)
			{
				Debug.Log("Warning. Path is missing Path2D component"); return;
			}

			movePath.SnapNodeToPath(this, force);
		}
	}
}
