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
        using GoStateMachineSharedPtr = std::shared_ptr<GoStateMachine<StateType>>;
        using GoStateMachinePtr = GoStateMachineSharedPtr const&;
        
        virtual void OnStateChange(GoStateMachinePtr inStateMachine) = 0;
        virtual void OnStateFinish(GoStateMachinePtr inStateMachine) = 0;
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
        using GoStateMachineSharedPtr = std::shared_ptr<GoStateMachine<StateType>>;
        using GoStateMachinePtr = GoStateMachineSharedPtr const&;
        
        GoStateMachineSharedPtr sm = std::make_shared<GoStateMachine<StateType>>();
        std::weak_ptr<SomeGoStateListener<StateType>> owner;

    protected:
        virtual void OnStateChange(GoStateMachinePtr inStateMachine) {
            if (owner.expired()) { return; }
            owner.lock()->OnStateChange(inStateMachine);
        }

        virtual void OnStateFinish(GoStateMachinePtr inStateMachine) {
            if (owner.expired()) { return; }
            owner.lock()->OnStateFinish(inStateMachine);
        }

        // Due to a limitation of C++ and shared_ptr, we must manually kickstart this using Go
        void GoInternal() override {
            Base::GoInternal();

            auto listener = std::dynamic_pointer_cast<SomeListener>(this->shared_from_this());
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

        std::shared_ptr<Broadcaster> broadcaster = std::make_shared<Broadcaster>();

        GoCore()
        {
        }

        GoCore(std::shared_ptr<SomeGoStateListener<StateType>> owner) : owner(owner)
        {
        }

        void OnUpdate(TimeSlice time) override
        {
            sm->OnUpdate(time);
        }

        bool IsFinished() const override { return false; }

        void OnEvent(EventPtr event) override
        {
            if (event->sentFrom.expired()) { return; }

            auto asStateMachine = std::dynamic_pointer_cast<GoStateMachine<StateType>>(event->sentFrom.lock());
            if (!asStateMachine) { return; }

            if (auto stateChangeEvent = std::dynamic_pointer_cast<EventStateChange<StateType>>(event))
            {
                OnStateChange(asStateMachine);
                return;
            }

            if (auto stateFinishEvent = std::dynamic_pointer_cast<EventStateFinish<StateType>>(event))
            {
                OnStateFinish(asStateMachine);
                return;
            }
        }
    };
}

#endif
