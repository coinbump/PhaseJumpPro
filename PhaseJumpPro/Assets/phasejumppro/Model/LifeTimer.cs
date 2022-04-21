using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple utility behavior
 * CODE REVIEW: 4/13/22
 */
namespace PJ
{
    /// <summary>
    /// Destroys an object after N seconds have passed
    /// Example: shotgun bullets have short lifespan
    /// </summary>
    public class LifeTimer : MonoBehaviour
    {
        public float lifeTime;

        protected float timer;

        void Update()
        {
            if (lifeTime <= 0) { return; }

            timer += Time.deltaTime;

            if (timer >= lifeTime)
            {
                Destroy(gameObject);
            }
        }
    }
}
