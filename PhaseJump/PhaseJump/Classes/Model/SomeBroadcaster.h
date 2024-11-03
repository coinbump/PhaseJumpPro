#pragma once

#include <memory>

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 10/6/24
 */
namespace PJ {
    class SomeListener;
    class SomeSignal;

    /// Interface to an object that sends signals to listeners
    class SomeBroadcaster {
    public:
        virtual ~SomeBroadcaster() {}

        /// Add a listener that will receive signal events
        virtual void Add(WP<SomeListener> listener) = 0;

        /// Remove a listener from the list that receives signal events
        virtual void Remove(WP<SomeListener> listener) = 0;

        /// Broadcast the signal to all listeners
        virtual void Broadcast(SomeSignal& signal) = 0;
    };
} // namespace PJ
