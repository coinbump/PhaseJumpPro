#ifndef PJLISTENER_H
#define PJLISTENER_H

#include "Event.h"

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
        virtual void OnListen(Event event) = 0;
    };
}

#endif
