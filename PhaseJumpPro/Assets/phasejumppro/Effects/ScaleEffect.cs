using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple animation utility
 * CODE REVIEW: 12/26/22
 */
namespace PJ
{
    /// <summary>
    /// When it receives or loses focus, will scale itself accordingly
    /// </summary>
    public class ScaleEffect : SomeAnimatedEffect
    {
        public float offScale = 1.0f;
        public float onScale = 2.0f;

        public override void UpdateEffectProperties()
        {
            base.UpdateEffectProperties();

            var valveState = valve.ValveState;
            var transformScale = offScale + (onScale - offScale) * valveState;
            transform.localScale = new Vector3(transformScale, transformScale, transformScale);
        }
    }
}
