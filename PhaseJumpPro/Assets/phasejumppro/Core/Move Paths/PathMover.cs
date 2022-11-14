using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/21/22
 */
namespace PJ
{
    /// <summary>
    /// Defines state and timing information for an object attached to a move path
    /// Usage:
    /// 1. Objects that move on a move path should be children of the move path object
    /// 2. Objects that move on a move path should have a PathMover component (or they won't move)
    /// </summary>
    public class PathMover : MonoBehaviour
    {
        /// <summary>
        /// Normalized position along path (0-1.0)
        /// </summary>
        public float startPosition;

        /// <summary>
        /// Time to complete one cycle from 0-1.0
        /// </summary>
        public float cycleTime = 3.0f;

        /// <summary>
        /// Object's relative mode speed on the path
        /// </summary>
        public float moveSpeed = 1.0f;

        /// <summary>
        /// Start state for the animation cycle
        /// </summary>
        public AnimationCycleState startCycleState;

        public Animator animator = new Animator();

        public float Progress
        {
            get => animator.Progress;
        }

        protected override void Awake()
        {
        }

        public virtual void Go()
        {
            animator.CycleTime = cycleTime / moveSpeed; // Must set cycle time before progress
            animator.SetProgress(startPosition);
            animator.CycleState = startCycleState;

            //Debug.Log("PathMover Go cycle time: " + animator.CycleTime.ToString());
        }

        protected override void Update()
        {
            var time = new TimeSlice(Time.deltaTime);
            animator.OnUpdate(time);

            //Debug.Log("PathMover Update progress " + Progress.ToString());
        }
    }
}
