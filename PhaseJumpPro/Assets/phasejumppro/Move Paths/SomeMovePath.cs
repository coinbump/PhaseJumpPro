using System.Collections;
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
    public abstract class SomeMovePath : WorldComponent
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

        /// <summary>
        /// Path model for this move path
        /// (override SnapPathMover if you don't want to use a path object)
        /// </summary>
        public SomePath path;

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
        protected virtual void SnapPathMover(PathMover pathMover, float progress, bool force)
        {
            if (null == path) { return; }
            var position = path.PositionAt(progress);
            MovePathMoverToLocalPosition(pathMover, position, force);
        }

        /// <summary>
        /// Override and return path, or return null and override SnapPathMover for manual paths
        /// </summary>
        protected abstract SomePath BuildPath();

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
            base.Awake();

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

            path = BuildPath();
        }

        protected override void Update()
        {
            var time = new TimeSlice(Time.deltaTime);

            foreach (Transform childTransform in transform)
            {
                var pathMover = childTransform.gameObject.GetComponent<PathMover>();
                if (null == pathMover)
                {
                    Debug.Log("Warning. Move Path requires PathMover component");
                    continue;
                }

                UpdatePathMover(pathMover, time);

                if (pathMover.TryGetComponent(out SomeNode node))
                {
                    // Move non physics objects in Update
                    if (node.IsKinematic)
                    {
                        //Debug.Log("Update Path snap");
                        SnapPathMover(pathMover, pathMover.Progress, false);
                    }
                }
                else
                {
                    SnapPathMover(pathMover, pathMover.Progress, false);
                }
            }
        }

        protected virtual void MovePathMoverToLocalPosition(PathMover pathMover, Vector3 localPosition, bool force)
        {
            if (force)
            {
                pathMover.transform.localPosition = localPosition;
                return;
            }

            if (pathMover.gameObject.TryGetComponent(out SomeNode node))
            {
                var worldPosition = transform.TransformPoint(localPosition);
                node.MoveToPosition(worldPosition, force);
            }
            else
            {
                pathMover.transform.localPosition = localPosition;
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

                if (childObject.TryGetComponent(out SomeNode node))
                {
                    if (!node.IsKinematic)
                    {
                        //Debug.Log("FixedUpdate Path snap");
                        SnapPathMover(pathMover, pathMover.Progress, false);
                    }
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
            path = BuildPath();
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

        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            EditorUtils.DrawPath(BuildPath(), 36, transform, renderState);
        }
#endif
    }
}
