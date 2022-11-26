﻿using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 11/26/22
*/
namespace PJ
{
	/// <summary>
    /// A move path defines a path for its children to move across
    /// </summary>
	public abstract class SomeMovePath : MonoBehaviour
	{
		public enum MoveSpeedType
        {
			// Object on a path moves from 0-1.0 in cycleTime/moveSpeed seconds
			Progress

			// Object on a path moves at moveSpeed velocity (not all paths support this)
			// FUTURE: Velocity
        }

		/// <summary>
		/// Determines who decides a move path property: the path or the PathMover attached to the child object
		/// </summary>
		public enum ComponentType
        {
			Path,

			Mover
        }

		public MoveSpeedType moveSpeedType;

		/// <summary>
        /// How objects cycle on this path: loop, once, or ping-pong
        /// </summary>
		public AnimationCycleType cycleType;

		/// <summary>
        /// Time for a complete cycle movement across the path
        /// </summary>
		public float cycleTime = 3.0f;

		/// <summary>
		/// Determines whether the path, or the objects on the path choose the cycle time
		/// </summary>
		public ComponentType cycleTimeComponent;

		/// <summary>
		/// Move the object that owns this path mover to its position on the path
		/// </summary>
		protected abstract void SnapPathMover(PathMover pathMover, float progress, bool force);

		/// <summary>
		/// Update the path mover over time
		/// </summary>
		protected virtual void UpdatePathMover(PathMover pathMover, TimeSlice time) { }

		public void SnapAllToStartPosition()
		{
			foreach (Transform childTransform in transform)
			{
                if (childTransform.TryGetComponent(out PathMover pathMover))
                {
                    SnapPathMover(pathMover, pathMover.startPosition, true);
                }
            }
		}

		protected override void Awake()
		{
			foreach (Transform childTransform in transform)
			{
                if (childTransform.TryGetComponent(out PathMover pathMover))
                {
                    pathMover.cycleTimer.CycleType = cycleType;

                    // Either the path chooses the cycle time, or the path mover does
                    switch (cycleTimeComponent)
                    {
                        case ComponentType.Path:
                            pathMover.CycleTime = cycleTime;
                            break;
                        case ComponentType.Mover:
                            break;
                    }
                }
            }
		}

		protected override void Update()
		{
			var time = new TimeSlice(Time.deltaTime);

			foreach (Transform childTransform in transform)
			{
				var pathMover = childTransform.gameObject.GetComponent<PathMover>();
				if (null == pathMover) {
					Debug.Log("Warning. Move Path requires PathMover component");
					continue;
				}

				UpdatePathMover(pathMover, time);

				var node = pathMover.GetComponent<SomeNode>();
				if (null == node) {
					Debug.Log("Error. Move Path requires SomeNode subclass");
					continue;
				}

				// Move non physics objects in Update
				if (node.IsKinematic)
				{
					//Debug.Log("Update Path snap");
					SnapPathMover(pathMover, pathMover.Progress, false);
				}
			}
		}

		protected virtual void MovePathMoverToLocalPosition(PathMover pathMover, Vector3 localPosition, bool force)
		{
			if (force)
            {
				pathMover.gameObject.transform.localPosition = localPosition;
				return;
            }
			
			var node = pathMover.gameObject.GetComponent<SomeNode>();
			if (null != node)
			{
				var worldPosition = transform.TransformPoint(localPosition);
				node.MoveToPosition(worldPosition, force);
			}
		}

		/// <summary>
		/// Move non kinematic objects in FixedUpdate
		/// </summary>
		protected virtual void FixedUpdate()
		{
			foreach (Transform childTransform in transform)
			{
				var childObject = childTransform.gameObject;
				var pathMover = childObject.GetComponent<PathMover>();
				if (null == pathMover) { continue; }

				var node = childObject.GetComponent<SomeNode>();
				if (null == node) { continue; }

				if (!node.IsKinematic)
				{
					//Debug.Log("FixedUpdate Path snap");
					SnapPathMover(pathMover, pathMover.Progress, false);
				}
			}
		}

		public virtual void SnapToPath(GameObject gameObject, bool force)
		{
			var pathMover = gameObject.GetComponent<PathMover>();
			if (null == pathMover) { return; }

			SnapPathMover(pathMover, pathMover.Progress, force);
		}

#if UNITY_EDITOR
        protected virtual void OnValidate()
        {
            SnapAllToStartPosition();
        }

        [CustomEditor(typeof(SomeMovePath), true)]
		public class Editor : UnityEditor.Editor
		{
			public override void OnInspectorGUI()
			{
				DrawDefaultInspector();

				if (GUILayout.Button("Snap"))
				{
					SomeMovePath movePath = (SomeMovePath)target;
					movePath.SnapAllToStartPosition();
				}
			}
		}
#endif
	}
}
