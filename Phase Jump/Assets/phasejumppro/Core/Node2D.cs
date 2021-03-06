﻿using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars. Simple utility class for 2D games with Unit Tests
 * CODE REVIEW: 4.8.18
 */
namespace PJ
{
	public enum UpdateType
	{
		Default, Fixed
	}

	/// <summary>
	/// Provides utility methods for simplifying common 2D game scenarios
	/// </summary>
	public class Node2D : MonoBehaviour
	{
        [Header("Node2D Properties")]

		private float _rotation;    // Simplifies basic 2D rotation
		public bool isKinematic;	// If true, motion is handled without rigidobdy/physics

		[HideInInspector]
		public Tags tags = new Tags();

		public string initialState;

		[Serializable]
		public struct NodeTag
		{
			public string name;
			public string value;    // Type is inferred from value
		}
		public List<NodeTag> nodeTags = new List<NodeTag>();

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

		public float rotationSpeed; // Normalized angles per second
		public bool dontModRotation = false;

		public enum CullType
		{
			None, OnInvisible
		}
		public CullType cullType = CullType.None;

		protected StateMachine<string> state = new StateMachine<string>();
		protected Rigidbody2D rb;

		/// <summary>
		/// Handles state machine.
		/// </summary>
		protected class Core : PJ.Core
		{
			public WeakReference<Node2D> Owner { get; }

			public Core(Node2D owner)
			{
				Owner = new WeakReference<Node2D>(owner);
				SetStateMachine(owner.state);
			}

			protected override void EvtStateChanged(SomeStateMachine state)
			{
				base.EvtStateChanged(state);

				if (!Owner.TryGetTarget(out Node2D owner)) { return; }
				if (null == owner) { return; }

				owner.EvtStateChanged(state);
			}

            protected override void EvtStateFinished(SomeStateMachine state)
            {
                base.EvtStateFinished(state);

				if (!Owner.TryGetTarget(out Node2D owner)) { return; }
				if (null == owner) { return; }

				owner.EvtStateFinished(state);
			}
		}

		protected Core core;

		// State
		public string NodeState
		{
			get
			{
				return state.State;
			}
			set
			{
				state.State = value;
			}
		}

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

		protected virtual void OnValidate()
		{
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

		protected virtual void Awake()
		{
			core = new Core(this);
			tags = new Tags();
		}

		protected virtual void Start()
		{
			rb = GetComponent<Rigidbody2D>();

			foreach (NodeTag tag in nodeTags)
			{
				tags.Add(tag.name, tag.value);
			}

			state.State = initialState;
		}

		protected virtual void Update()
		{
			UpdateNode(UpdateType.Default);
		}

		protected virtual void FixedUpdate()
		{
			UpdateNode(UpdateType.Fixed);
		}

		protected virtual void OnBecameInvisible()
		{
			switch (cullType)
			{
				case CullType.OnInvisible:
					Destroy(this.gameObject);
					break;
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

		public bool ShouldMoveForUpdate(UpdateType updateType)
		{
			bool isFixedUpdate = updateType == UpdateType.Fixed;

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

		protected void UpdateNode(UpdateType updateType)
		{
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

		public void MoveToPosition(Vector2 position, bool force = false)
		{
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

		public bool HasTag(string name) => tags.ContainsKey(name) || gameObject.CompareTag(name);

		protected virtual void EvtStateChanged(SomeStateMachine state)
		{
			// Implemented by subclass
		}

		protected virtual void EvtStateFinished(SomeStateMachine state)
		{
			// Implemented by subclass
		}
	}
}