using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple animation utility
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    /// <summary>
    /// When it receives or loses focus, will scale itself accordingly
    /// </summary>
    public class ScaleEffect : AnimatedEffect
    {
        public float offScale = 1.0f;
        public float onScale = 2.0f;

        protected override void UpdateAnimatableProperties()
        {
            base.UpdateAnimatableProperties();

            var valveState = valve.ValveState;
            var transformScale = offScale + (onScale - offScale) * valveState;
            transform.localScale = new Vector3(transformScale, transformScale, transformScale);
        }
    }
}
