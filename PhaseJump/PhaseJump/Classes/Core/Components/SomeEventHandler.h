#ifndef PJSOMEEVENTHANDLER_H
#define PJSOMEEVENTHANDLER_H

#include "WorldComponent.h"
#include "SomeListener.h"
#include "Macros.h"

namespace PJ {
    class Event;

    /// <summary>
    /// A component that listens to events
    /// </summary>
    class SomeEventHandler : public WorldComponent, public SomeListener
    {
    public:
    };
}

#endif
