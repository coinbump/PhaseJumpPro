#ifndef PJSOMEBROADCASTER_H
#define PJSOMEBROADCASTER_H

#include <memory>

namespace PJ {
    class SomeListener;

    class SomeBroadcaster {
        virtual void AddListener(WP<SomeListener> listener) = 0;
        virtual void RemoveListener(WP<SomeListener> listener) = 0;
    };
}

#endif
