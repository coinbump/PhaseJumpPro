using System;

/*
 * RATING: 5 stars
 * Simple design pattern
 * CODE REVIEW: 4/13/22
 */
namespace PJ
{
    /// <summary>
    /// An event can only occur up to N times, until those slots
    /// are emptied via Remove
    /// Example: game where there are a limited # of bullets allowed at a time
    /// </summary>
    public class CounterLimiter : SomeLimiter
    {
        public int maxCount;
        public int count;

        public CounterLimiter(int maxCount)
        {
            this.maxCount = maxCount;
            count = 0;
        }

        public void Add()
        {
            count++;
        }

        public void Remove()
        {
            count = Math.Max(0, count - 1);
        }

        public override bool CanFire()
        {
            return count < maxCount;
        }

        public override void OnUpdate(TimeSlice time)
        {
        }

        protected override void OnFire()
        {
            Add();
        }
    }
}
