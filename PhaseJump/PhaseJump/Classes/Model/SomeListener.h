#pragma once

/*
 RATING: 5+ stars
 Simple protocol
 CODE REVIEW: 4/21/26
 */
namespace PJ {
    class SomeSignal;

    /// Receives events from broadcasters
    class SomeListener {
    public:
        virtual ~SomeListener() {}

        /// Called when a signal is sent to this object
        virtual void OnSignal(SomeSignal const& signal) = 0;
    };
} // namespace PJ
