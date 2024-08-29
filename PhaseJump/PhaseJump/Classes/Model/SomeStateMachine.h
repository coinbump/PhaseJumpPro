#pragma once

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    template <class StateType>
    class SomeStateMachine {
    public:
        virtual ~SomeStateMachine() {}

        virtual StateType State() const = 0;
    };
} // namespace PJ
