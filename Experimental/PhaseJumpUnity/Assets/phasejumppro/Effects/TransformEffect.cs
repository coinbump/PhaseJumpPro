using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple animation utility
 * CODE REVIEW: 6/10/23
 */
namespace PJ
{
    /// <summary>
    /// Interpolates its transform properties when it receives or loses focus
    /// </summary>
    public class TransformEffect : SomeAnimatedEffect
    {
        public GeoTransform offTransform = GeoTransform.defaultTransform;
        public GeoTransform onTransform = GeoTransform.defaultTransform;

        /// <summary>
        /// Unparented transforms can cause problems if we try to modify the position.
        /// If true, do not modify the position.
        /// </summary>
        public bool ignorePosition = false;

        public override void UpdateEffectProperties()
        {
            base.UpdateEffectProperties();

            var valveState = valve.ValveState;

            var transformScale = offTransform.scale + (onTransform.scale - offTransform.scale) * valveState;
            transform.localScale = transformScale;

            var transformRotation = offTransform.rotation + (onTransform.rotation - offTransform.rotation) * valveState;
            transform.localRotation = Quaternion.Euler(transformRotation.x, transformRotation.y, transformRotation.z);

            if (!ignorePosition)
            {
                var transformPosition = offTransform.position + (onTransform.position - offTransform.position) * valveState;
                transform.localPosition = transformPosition;
            }
        }
    }
}
