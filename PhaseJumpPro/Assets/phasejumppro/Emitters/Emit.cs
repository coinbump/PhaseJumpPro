using UnityEngine;
using System;

/*
RATING: 5 stars
Simple type
CODE REVIEW: 12/31/22
*/
namespace PJ
{
    /// <summary>
    /// Model for an emission
    /// </summary>
    public class Emit
    {
        // Direction + velocity
        public Vector3 velocity;

        // Offset from spawn origin
        public Vector3 offset;

        // Delay before showing (if > 0)
        public float delay;

        // Delay before destroying (if > 0)
        public float lifeTime;

        public Emit(Vector3 velocity, Vector3 offset, float delay, float lifeTime)
        {
            this.velocity = velocity;
            this.offset = offset;
            this.delay = delay;
            this.lifeTime = lifeTime;
        }
    }
}
