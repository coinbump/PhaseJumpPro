#ifndef PJLISTENER_H
#define PJLISTENER_H

#include "Event.h"
#include <memory>

/*
 RATING: 5 stars
 Utility model
 CODE REVIEW: 11/20/22
 */
namespace PJ {
    /// <summary>
    /// Receives events from broadcasters
    /// </summary>
    class SomeListener
    {
    public:
        using EventSharedPtr = std::shared_ptr<Event>;
        using EventPtr = EventSharedPtr const&;

        virtual void OnEvent(EventPtr event) = 0;
    };
}

#endif
