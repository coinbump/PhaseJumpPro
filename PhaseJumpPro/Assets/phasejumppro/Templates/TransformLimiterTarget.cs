using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple behavior
 * CODE REVIEW: 4/14/22
 */
namespace PJ
{
    public class TransformLimiterTarget : MonoBehaviour
    {
        public GameObject transformLimiter;
        public bool isEnabled = true;

        // FUTURE: support sample recording previous positions for smart limiters if needed
        public void LateUpdate()
        {
            if (null == this.transformLimiter) { return; }
            if (!isEnabled) { return;  }

            var transformLimiter = this.transformLimiter.GetComponent<SomeTransformLimiter>();
            if (null == transformLimiter) { return; }

            transformLimiter.Scan(gameObject);
        }
    }
}
