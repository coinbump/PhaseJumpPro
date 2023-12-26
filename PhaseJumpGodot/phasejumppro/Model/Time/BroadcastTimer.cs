using System;

/*
 * RATING: 5 stars
 * Simple timer
 * CODE REVIEW: 4/23/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// After it finishes, sends a signal event. Useful for repeat-delay events
    /// Example: dealing cards. A signal timer sends an event to deal the next card
    /// </summary>
    public class BroadcastTimer : PJ.Timer
    {
        public class Event : PJ.Event
        {
            public Event(string id)
            {
                this.id = id;
            }
        }

        public Broadcaster broadcaster = new Broadcaster();

        public BroadcastTimer(string id, float duration, RunType runType) : base(duration, runType)
        {
            this.id = id;
        }

        protected override void OnFinish()
        {
            base.OnFinish();

            broadcaster.Broadcast(new Event(id));
        }
    }

    /// <summary>
    /// After it finishes, sends a signal event. Useful for repeat-delay events
    /// Example: dealing cards. A signal timer sends an event to deal the next card
    /// </summary>
    public class BroadcastEventTimer<Event> : PJ.Timer where Event : PJ.Event, new()
    {
        public Broadcaster broadcaster = new Broadcaster();
        public Func<Event> allocator;

        public BroadcastEventTimer(float duration, RunType runType, Func<Event> allocator) : base(duration, runType)
        {
            this.allocator = allocator;
        }

        protected override void OnFinish()
        {
            base.OnFinish();

            broadcaster.Broadcast(allocator());
        }
    }
}
