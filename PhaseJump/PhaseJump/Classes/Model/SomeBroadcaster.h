#ifndef PJSOMEBROADCASTER_H
#define PJSOMEBROADCASTER_H

class SomeListener;

namespace PJ {
    class SomeBroadcaster {
        virtual void AddListener(SomeListener listener) = 0;
        virtual void RemoveListener(SomeListener listener) = 0;
    };
}

#endif
