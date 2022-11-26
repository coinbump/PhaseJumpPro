
/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 11/26/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Limits an action until N seconds have passed after the last time it happened
    /// Example: bulllets only fire at a certain rate, rapid fire power up changes that
    /// </summary>
    public class RateLimiter : SomeLimiter
    {
        public float minDelta = 1.0f;
        public float timer = 0;

        public RateLimiter(float minDelta)
        {
            this.minDelta = minDelta;
        }

        public override bool CanFire()
        {
            return timer >= minDelta;
        }

        public override void OnUpdate(TimeSlice time)
        {
            timer += time.delta;
        }

        protected override void OnFire()
        {
            timer = 0;
        }
    }
}
