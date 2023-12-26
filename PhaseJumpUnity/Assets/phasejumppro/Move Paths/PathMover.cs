using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 11/26/22
*/
namespace PJ
{
    /// <summary>
    /// Defines state and timing information for an object attached to a move path
    /// Usage:
    /// 1. Objects that move on a move path should be children of the move path object
    /// 2. Objects that move on a move path should have a PathMover component (or they won't move)
    /// </summary>
    public class PathMover : WorldComponent
    {
        /// <summary>
        /// Normalized position along path (0-1.0)
        /// </summary>
        [Range(0, 1.0f)]
        public float startPosition;

        /// <summary>
        /// Time to complete one cycle from 0-1.0
        /// </summary>
        [SerializeField]
        protected float cycleTime = 3.0f;

        /// <summary>
        /// Object's relative mode speed on the path
        /// </summary>
        public float moveSpeed = 1.0f;

        public float CycleTime
        {
            get => cycleTime;
            set
            {
                cycleTime = value;
                if (null != cycleTimer)
                {
                    cycleTimer.CycleTime = cycleTime / moveSpeed;
                }
            }
        }

        /// <summary>
        /// Start state for the animation cycle
        /// </summary>
        public AnimationCycleState startCycleState;

        public AnimationCycleTimer cycleTimer;

        public float Progress
        {
            get => cycleTimer.Progress;
        }

        public PathMover()
        {
            cycleTimer = new AnimationCycleTimer(cycleTime / moveSpeed, AnimationCycleType.PingPong);
        }

        protected override void Start()
        {
            base.Start();

            cycleTimer.CycleTime = cycleTime;
            cycleTimer.CycleState = startCycleState;
            cycleTimer.SetProgress(startPosition);
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);
            cycleTimer.OnUpdate(time);

            //Debug.Log("PathMover Update progress " + Progress.ToString());
        }

#if UNITY_EDITOR
        protected virtual void OnValidate()
        {
            var parent = transform.parent;
            if (null == parent) { return; }

            if (parent.gameObject.TryGetComponent(out SomeMovePath movePath))
            {
                movePath.SnapAllToStartPosition();
            }
        }
#endif
    }
}
