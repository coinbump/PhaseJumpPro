#ifndef PJBROADCASTER_H
#define PJBROADCASTER_H

#include "SomeBroadcaster.h"
#include "SomeListener.h"
#include "Utils.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/5/23
 */
namespace PJ {
    /// Broadcaster sends messages to listeners.
    class Broadcaster : public SomeBroadcaster {
    public:
        // TODO: why WP?
        using ListenerWeakPtr = WP<SomeListener>;
        using ListenerList = VectorList<ListenerWeakPtr>;
        using EventSharedPtr = SP<SomeEvent>;
        using EventPtr = EventSharedPtr const&;

        ListenerList listeners;

        Broadcaster() {}

        void RemoveListener(ListenerWeakPtr listener) override {
            for (auto i = listeners.begin(); i != listeners.end(); i++) {
                auto _listener = *i;
                if (_listener.expired()) {
                    continue;
                }

                if (_listener.lock() == listener.lock()) {
                    listeners.erase(i);
                    return;
                }
            }
        }

        void AddListener(ListenerWeakPtr listener) override {
            if (listener.expired()) {
                return;
            }
            Add(listeners, listener);
        }

        void Clear() {
            listeners.clear();
        }

        void Broadcast(EventPtr event) {
            ListenerList activeListeners;

            // TODO: potential to crash
            for (auto listener : listeners) {
                if (listener.expired()) {
                    continue;
                }
                Add(activeListeners, listener);

                listener.lock()->OnEvent(event);
            }

            listeners = activeListeners;
        }
    };
} // namespace PJ

#endif
