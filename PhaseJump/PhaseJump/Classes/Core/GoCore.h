#ifndef PJGOCORE_H
#define PJGOCORE_H

#include "GoStateMachine.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/20/22
 */
namespace PJ
{
    template <class StateType>
    class SomeGoStateListener
    {
    public:
        using GoStateMachineSharedPtr = SP<GoStateMachine<StateType>>;
        using GoStateMachinePtr = GoStateMachineSharedPtr const&;
        
        virtual void OnStateChange(GoStateMachine<StateType>& inStateMachine) = 0;
        virtual void OnStateFinish(GoStateMachine<StateType>& inStateMachine) = 0;
    };

    /// <summary>
    /// Provides common design patterns:
    /// - Broadcaster, for broadcast-listen events
    /// - State machine, for state changes
    /// </summary>
    template <class StateType>
    class GoCore : public Core, public Updatable, public SomeListener
    {
    public:
        using Base = Core;
        using GoStateMachineSharedPtr = SP<GoStateMachine<StateType>>;
        using GoStateMachinePtr = GoStateMachineSharedPtr const&;
        
        GoStateMachineSharedPtr sm = MAKE<GoStateMachine<StateType>>();
        WP<SomeGoStateListener<StateType>> owner;

    protected:
        virtual void OnStateChange(GoStateMachine<StateType>& inStateMachine) {
            if (owner.expired()) { return; }
            owner.lock()->OnStateChange(inStateMachine);
        }

        virtual void OnStateFinish(GoStateMachine<StateType>& inStateMachine) {
            if (owner.expired()) { return; }
            owner.lock()->OnStateFinish(inStateMachine);
        }

        // Due to a limitation of C++ and shared_ptr, we must manually kickstart this using Go
        void GoInternal() override {
            Base::GoInternal();

            auto listener = DCAST<SomeListener>(this->shared_from_this());
            sm->AddListener(listener);
        }

    public:
        GoStateMachineSharedPtr& StateMachine() { return sm; }
        GoStateMachineSharedPtr const& StateMachine() const { return sm; }

        StateType State() const { return sm->State(); }
        void SetState(StateType value)
        {
            sm->SetState(value);
        }

        Broadcaster broadcaster;

        GoCore()
        {
        }

        GoCore(SP<SomeGoStateListener<StateType>> owner) : owner(owner)
        {
            sm.AddListener(owner);
        }

        void OnUpdate(TimeSlice time) override
        {
            sm->OnUpdate(time);
        }

        bool IsFinished() const override { return false; }

        /// Call to configure the listeners and owner-core relationship
        void Configure(SP<SomeGoStateListener<StateType>> owner) {
            if (!this->owner.expired()) { return; }
            
            auto listener = DCAST<SomeListener>(this->shared_from_this());
            sm->AddListener(listener);

            this->owner = owner;
        }

        void OnEvent(SPC<Event> event) override
        {
            if (event->sentFrom.expired()) { return; }

            auto asStateMachine = DCAST<GoStateMachine<StateType>>(event->sentFrom.lock());
            if (!asStateMachine) { return; }

            if (auto stateChangeEvent = DCAST<EventStateChange<StateType>>(event))
            {
                OnStateChange(*asStateMachine);
                return;
            }

            if (auto stateFinishEvent = DCAST<EventStateFinish<StateType>>(event))
            {
                OnStateFinish(*asStateMachine);
                return;
            }
        }
    };
}

#endif
