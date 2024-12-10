#pragma once

#include "SignalTypes.h"
#include "StringUtils.h"
#include "Utils.h"
#include "Void.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/14/24
 */
namespace PJ {
    class Base;

    /// Signals are sent as communication between objects
    /// Signal: "Here's something you need to know"
    /// Event: "Here's something that happened"
    class SomeSignal {
    public:
        virtual ~SomeSignal() {}
    };

    /// Used to send signal events from one object to another
    template <class Core = Void>
    class Event : public SomeSignal {
    public:
        Core core{};

        Event(Core core = {}) :
            core(core) {}
    };
} // namespace PJ
