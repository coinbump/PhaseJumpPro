#ifndef PJSOMEBROADCASTER_H
#define PJSOMEBROADCASTER_H

#include <memory>

namespace PJ {
    class SomeListener;

    class SomeBroadcaster {
        virtual void AddListener(std::weak_ptr<SomeListener> listener) = 0;
        virtual void RemoveListener(std::weak_ptr<SomeListener> listener) = 0;
    };
}

#endif
