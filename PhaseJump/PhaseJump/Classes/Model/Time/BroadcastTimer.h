#ifndef PJBROADCASTTIMER_H
#define PJBROADCASTTIMER_H

#include "Timer.h"
#include "Broadcaster.h"
#include "Event.h"
#include "_String.h"
#include <memory>

/*
 RATING: 5 stars
 Simple timer
 CODE REVIEW: 2/18/23
 */
namespace PJ
{
    /// <summary>
    /// After it finishes, sends a signal event. Useful for repeat-delay events
    /// Example: dealing cards. A signal timer sends an event to deal the next card
    /// </summary>
    class BroadcastTimer : public Timer
    {
    public:
        using Base = Timer;

        class Event : public PJ::Event
        {
        public:
            Event(String id) : PJ::Event(id)
            {
            }
        };

        Broadcaster broadcaster;

        BroadcastTimer(String id, float duration, RunType runType) : Base(duration, runType)
        {
            this->id = id;
        }

    protected:
        void OnFinish() override
        {
            Base::OnFinish();

            auto event = MAKE<Event>(id);
            broadcaster.Broadcast(SCAST<PJ::Event>(event));
        }
    };

    /// <summary>
    /// After it finishes, sends a signal event. Useful for repeat-delay events
    /// Example: dealing cards. A signal timer sends an event to deal the next card
    /// </summary>
    template <class Event>
    class BroadcastEventTimer : Timer
    {
    public:
        using Base = Timer;
        std::function<SP<Event>()> allocator;

        Broadcaster broadcaster;

        BroadcastEventTimer(float duration, RunType runType, std::function<SP<Event>()> allocator) : Base(duration, runType), allocator(allocator)
        {
        }

    protected:
        void OnFinish() override
        {
            Base::OnFinish();

            broadcaster.Broadcast(allocator());
        }
    };
}

#endif
