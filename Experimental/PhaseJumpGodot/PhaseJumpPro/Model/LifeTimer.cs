using System;
using Godot;

/*
 * RATING: 5 stars
 * Simple utility behavior
 * CODE REVIEW: 4/13/22
 */
// TODO: Get rid of this? Use Destroy(, time) instead?
namespace PJ
{
    /// <summary>
    /// Destroys an object after N seconds have passed
    /// Example: shotgun bullets have short lifespan
    /// </summary>
    public partial class LifeTimer : WorldNode
    {
        public float lifeTime;

        protected float timer;

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (lifeTime <= 0) { return; }

            timer += time.delta;

            if (timer >= lifeTime)
            {
                QueueFree();
            }
        }
    }
}
