using System;
using UnityEngine;

/*
RATING: 5 stars
Simple utility
CODE REVIEW: 1/14/23
*/
namespace PJ
{
    public class SomeSimAgent : Updatable
    {
        public WeakReference<SimAgentGroup> group;
        protected bool isActive = false;

        /// <summary>
        /// Agents are stored in a fixed-index array to provide maximum efficiency for large simulations
        /// </summary>
        public int index = -1;

        public bool IsActive
        {
            get => isActive;
            set => isActive = value;
        }

        public virtual void OnUpdate(TimeSlice time) { }
        public virtual bool IsFinished => false;

        /// <summary>
        /// (OPTIONAL) Step-based simulations operate based on fixed time steps
        /// </summary>
        public virtual void OnStep() { }
    }
}
