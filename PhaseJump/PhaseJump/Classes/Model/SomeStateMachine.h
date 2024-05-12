#ifndef PJSOMESTATEMACHINE_H
#define PJSOMESTATEMACHINE_H

/*
 RATING: 5 stars
 Simple interface
 CODE REVIEW: 12/11/22
 */
namespace PJ
{
    template <class StateType>
    class SomeStateMachine
    {
    public:
        virtual ~SomeStateMachine() {}
        
        virtual StateType State() const = 0;
    };
}

#endif
