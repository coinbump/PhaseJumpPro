#include "Broadcaster.h"

using namespace std;
using namespace PJ;

void Broadcaster::Broadcast(SomeSignal& signal) {
    auto iterListeners = listeners;
    for (auto& listener : iterListeners) {
        GUARD_CONTINUE(!listener.expired())
        listener.lock()->OnSignal(signal);
    }

    /// Don't refresh active listeners until after we broadcast, because new listeners could have
    /// been introduced during the broadcast loop
    ListenerList activeListeners;
    for (auto& listener : listeners) {
        GUARD_CONTINUE(!listener.expired())
        activeListeners.push_back(listener);
    }

    listeners = activeListeners;
}

void Broadcaster::Remove(WP<SomeListener> listener) {
    RemoveFirstIf(listeners, [&](auto& _listener) { return listener.lock() == _listener.lock(); });
}

void Broadcaster::Add(WP<SomeListener> listener) {
    GUARD(!listener.expired())
    listeners.push_back(listener);
}
