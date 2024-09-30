#pragma once

#include "List.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 9/22/24
 */
namespace PJ {
    class SomeUIEvent;

    /// Some world systems use event polling (SDL)
    class SomeUIEventPoller {
    public:
        enum class Status {
            Running,

            Done
        };

        struct Result {
            Status status;
            List<SP<SomeUIEvent>> uiEvents;

            Result(Status status, List<SP<SomeUIEvent>> uiEvents) :
                status(status),
                uiEvents(uiEvents) {}
        };

        virtual ~SomeUIEventPoller() {}

        virtual Result PollUIEvents() = 0;
    };
} // namespace PJ
