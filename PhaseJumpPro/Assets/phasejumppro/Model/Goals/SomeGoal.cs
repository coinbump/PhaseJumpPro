using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 12/29/22
 */
namespace PJ
{
    /// <summary>
    /// Goals are common in games to achieve progress.
    /// EXAMPLE: complete a task N times, achieve N experience, get to the destination, etc.
    /// </summary>
    public abstract class SomeGoal
    {
        public string id;

        protected bool isComplete;

        public bool IsComplete
        {
            get => isComplete;
            set
            {
                if (isComplete == value) { return; }
                isComplete = value;
                if (isComplete)
                {
                    OnComplete();
                }
            }
        }

        public Broadcaster broadcaster = new Broadcaster();

        public abstract float Progress { get; }

        public virtual void OnComplete()
        {
            broadcaster.Broadcast(new EventGoalComplete(this));
        }
    }
}
