#pragma once

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/9/24
 */
namespace PJ {
    /// Component that handles state changes
    template <class StateType>
    class SomeStateHandler : public WorldComponent<> {
    public:
        virtual void OnStateChange(StateType state) = 0;
    };
} // namespace PJ
