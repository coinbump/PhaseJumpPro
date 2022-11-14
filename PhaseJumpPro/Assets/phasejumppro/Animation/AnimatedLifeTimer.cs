using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/20/22
 */
namespace PJ
{
    /// <summary>
    /// Destroys the object after N seconds
    /// When the object reaches the trigger time (0-1.0) of its total lifetime, the exit animation is enabled
    /// </summary>
    public class AnimatedLifeTimer : MonoBehaviour
    {
        public SomeAnimation exitAnimation;
        public float lifeTime;
        
        [Range(0, 1.0f)]
        public float exitAnimationTrigger;

        protected float timer;

        protected override void Update()
        {
            if (lifeTime <= 0) { return; }

            timer += Time.deltaTime;

            float progress = timer / lifeTime;
            if (progress > exitAnimationTrigger)
            {
                if (null != exitAnimation)
                {
                    exitAnimation.enabled = true;
                }
                else
                {
                    Debug.Log("Error. No exit animation");
                }
            }

            if (timer >= lifeTime)
            {
                Destroy(gameObject);
            }
        }
    }
}
