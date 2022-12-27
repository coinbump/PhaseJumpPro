using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Simple animation
 * CODE REVIEW: 12/26/22
 */
namespace PJ
{
    public class AnimateRotationEffect2D : SomeEffect
    {
        /// <summary>
        /// Angles per second
        /// </summary>
        public float anglesPerSecond = 360.0f;

        protected NodeCore2D node;

        protected override void Start()
        {
            node = GetComponent<NodeCore2D>();
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (!IsOn) { return; }

            var eulerAngles = transform.eulerAngles;
            eulerAngles.z -= anglesPerSecond * Time.deltaTime;
            transform.localEulerAngles = eulerAngles;
        }
    }
}
