#ifndef PJSOMESTATEMACHINE_H
#define PJSOMESTATEMACHINE_H

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 5/18/24
 */
namespace PJ {
    template <class StateType>
    class SomeStateMachine {
    public:
        virtual ~SomeStateMachine() {}

        virtual StateType State() const = 0;
    };
} // namespace PJ

#endif
