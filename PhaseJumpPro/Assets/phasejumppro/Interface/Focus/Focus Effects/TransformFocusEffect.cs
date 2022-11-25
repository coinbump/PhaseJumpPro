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
    /// Interpolates its transform properties when it receives or loses focus
    /// </summary>
    public class TransformFocusEffect: AnimatedFocusEffect
    {
        public GeoTransform offTransform = GeoTransform.defaultTransform;
        public GeoTransform onTransform = GeoTransform.defaultTransform;

        protected override void Start()
        {
            base.Start();

            UpdateAnimatableProperties();
        }

        protected override void Update()
        {
            base.Update();

            UpdateAnimatableProperties();
        }

        protected override void UpdateAnimatableProperties()
        {
            var valveState = focusValve.ValveState;

            var transformScale = offTransform.scale + (onTransform.scale - offTransform.scale) * valveState;
            transform.localScale = transformScale;

            var transformRotation = offTransform.rotation + (onTransform.rotation - offTransform.rotation) * valveState;
            transform.localRotation = Quaternion.Euler(transformRotation.x, transformRotation.y, transformRotation.z);

            var transformPosition = offTransform.position + (onTransform.position - offTransform.position) * valveState;
            transform.localPosition = transformPosition;
        }
    }
}
