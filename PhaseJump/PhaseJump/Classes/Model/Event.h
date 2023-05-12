#ifndef PJEVENT_H
#define PJEVENT_H

#include "_String.h"
#include "Macros.h"
#include <memory>

/*
 RATING: 5 stars
 Utility model
 CODE REVIEW: 11/16/22
 */
namespace PJ {
    class Base;

    /// <summary>
    /// Used to send event from one object to another
    /// </summary>
    /// Naming convention: EventName or Events.Name. Avoids polluting the namespace with
    /// a large amount of event objects.
    class Event
    {
    public:
        // OPTIONAL:
        WP<Base> sentFrom;

        /// <summary>
        /// Event id
        /// </summary>
        String id;

        Event() {
        }

        Event(String id) : id(id) {
        }

        Event(String id, WP<Base> sentFrom) : id(id), sentFrom(sentFrom)
        {
        }

        virtual ~Event() {}
    };
}

#endif
