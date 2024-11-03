#pragma once

#include "SomeBroadcaster.h"
#include "SomeListener.h"
#include "Utils.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/6/24
 */
namespace PJ {
    /// Broadcaster sends messages to listeners.
    class Broadcaster : public SomeBroadcaster {
    public:
        using Base = SomeBroadcaster;
        using This = Broadcaster;
        using ListenerList = VectorList<WP<SomeListener>>;

    protected:
        ListenerList listeners;

    public:
        void Clear() {
            listeners.clear();
        }

        size_t Count() const {
            return listeners.size();
        }

        // MARK: SomeBroadcaster

        void Remove(WP<SomeListener> listener) override;
        void Add(WP<SomeListener> listener) override;
        void Broadcast(SomeSignal& signal) override;
    };
} // namespace PJ
