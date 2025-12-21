#pragma once

#include "SignalTypes.h"
#include "StringUtils.h"
#include "Utils.h"
#include "Void.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/27/24
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
    class CoreSignal : public SomeSignal {
    public:
        Core core{};

        CoreSignal(Core const& core = {}) :
            core(core) {}
    };

    /// Convenience name
    template <class Core = Void>
    using Event = CoreSignal<Core>;
} // namespace PJ
