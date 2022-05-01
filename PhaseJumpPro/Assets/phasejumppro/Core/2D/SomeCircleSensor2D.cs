using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 5/1/22
 */
namespace PJ
{
    /// <summary>
    /// A 2D sensor that uses a circle collider and checks for occlusions
    /// occluderCheckAngleStep determine the accuracy (more angle checks cost more, but are more accurate)
    /// </summary>
    public abstract class SomeCircleSensor2D : SomeSensor2D
    {
        protected CircleCollider2D circleCollider;

        public float MaxOccluderRaycastDistance => circleCollider.radius;

        /// <summary>
        /// Setup the circle collider
        /// </summary>
        protected override void Awake()
        {
            base.Awake();

            circleCollider = GetComponent<CircleCollider2D>();
            if (null == circleCollider)
            {
                circleCollider = gameObject.AddComponent<CircleCollider2D>();
            }

            circleCollider.isTrigger = true;
        }
    }
}
