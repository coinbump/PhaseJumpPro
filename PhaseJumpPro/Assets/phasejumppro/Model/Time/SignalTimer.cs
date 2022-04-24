using System;

/*
 * RATING: 5 stars
 * Simple timer
 * CODE REVIEW: 4/23/22
 */
namespace PJ
{
    /// <summary>
    /// After it finishes, sends a signal event. Useful for repeat-delay events
    /// Example: dealing cards. A signal timer sends an event to deal the next card
    /// </summary>
    public class SignalTimer : Timer
    {
        public class Event : PJ.Event
        {
            public Event(string identifier)
            {
                this.identifier = identifier;
            }
        }

        public Broadcaster broadcaster = new Broadcaster();

        public SignalTimer(string identifier, float duration, RunType runType) : base(duration, runType)
        {
            this.identifier = identifier;
        }

        protected override void OnFinish()
        {
            base.OnFinish();

            broadcaster.Broadcast(new Event(identifier));
        }
    }

    /// <summary>
    /// After it finishes, sends a signal event. Useful for repeat-delay events
    /// Example: dealing cards. A signal timer sends an event to deal the next card
    /// </summary>
    public class SignalEventTimer<Event> : Timer where Event : PJ.Event, new()
    {
        public Broadcaster broadcaster = new Broadcaster();

        public SignalEventTimer(float duration, RunType runType) : base(duration, runType)
        {
        }

        protected override void OnFinish()
        {
            base.OnFinish();

            broadcaster.Broadcast(new Event());
        }
    }
}
