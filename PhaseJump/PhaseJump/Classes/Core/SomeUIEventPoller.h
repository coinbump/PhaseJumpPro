#pragma once

#include "SomeUIEvent.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 10/20/24
 */
namespace PJ {
    class SomeUIEvent;

    /// Some world systems use event polling (Example: SDL)
    class SomeUIEventPoller {
    public:
        enum class StateType {
            /// Poller is actively running
            Run,

            /// Poller is finished running
            Finished
        };

        /// Result returned by polling method
        struct Result {
            StateType state;
            UIEventList uiEvents;

            Result(StateType state, UIEventList const& uiEvents) :
                state(state),
                uiEvents(uiEvents) {}
        };

        virtual ~SomeUIEventPoller() {}

        /// Override to poll UI events for this event loop
        virtual Result PollUIEvents() = 0;
    };
} // namespace PJ
