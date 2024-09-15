#ifndef PJSOMEEVENTHANDLER_H
#define PJSOMEEVENTHANDLER_H

#include "SomeListener.h"
#include "Utils.h"
#include "WorldComponent.h"

// CODE REVIEW: ?/23
namespace PJ {
    class SomeSignal;

    /// A component that listens to events
    // TODO: is this being used via inheritance anymore? Do we need it?
    class SomeEventHandler : public WorldComponent<>, public SomeListener {
    public:
    };
} // namespace PJ

#endif
