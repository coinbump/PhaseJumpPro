#ifndef PJSOMEEVENTPOLLER_H
#define PJSOMEEVENTPOLLER_H

#include "List.h"
#include "Utils.h"
#include <memory>

// CODE REVIEW: ?/23
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

#endif
