#ifndef PJSOMEEVENTPOLLER_H
#define PJSOMEEVENTPOLLER_H

#include "VectorList.h"
#include <memory>

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
            VectorList<std::shared_ptr<SomeUIEvent>> uiEvents;

            Result(Status status, VectorList<std::shared_ptr<SomeUIEvent>> uiEvents) : status(status), uiEvents(uiEvents) {
            }
        };

        virtual Result PollUIEvents() = 0;
    };
}

#endif
