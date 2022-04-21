using UnityEngine;
using System;

/*
 * RATING: 4 stars
 * Tested and works
 * CODE REVIEW: 4/14/22
 */
namespace PJ
{
    public abstract class SomeTransformLimiter2D : SomeTransformLimiter
    {
        protected override void OnPositionUpdated(GameObject target)
        {
            //Debug.Log("OnPositionUpdated");

            base.OnPositionUpdated(target);

            var node = target.GetComponent<Node2D>();
            if (null == node)
            {
                Debug.Log("Limiter2D requires Node2D");
                return;
            }

            node.OnTransformLimited();
        }
    }
}
