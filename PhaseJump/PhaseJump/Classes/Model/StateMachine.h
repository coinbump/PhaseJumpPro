#pragma once

#include "Broadcaster.h"
#include "CyclicGraph.h"
#include "Dev.h"
#include "Funcs.h"
#include "List.h"
#include "SomeStateMachine.h"
#include "StandardEventCore.h"
#include "UnorderedMap.h"
#include <iostream>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// Specifies how published values broadcast after set
    enum PublishValueCondition {
        /// Publish value when it changes
        OnChange,

        /// Always publish a value when it is set, even if the value doesn't change
        Always
    };

    struct StateMachineEdgeCore {
        using InputList = VectorList<String>;

        /// Inputs that cause a transition to the next state
        InputList inputs;

        StateMachineEdgeCore(InputList inputs) :
            inputs(inputs) {}
    };

    template <class StateType>
    using StateGraph = CyclicGraph<CyclicGraphNode<StateMachineEdgeCore, StateType>>;

    /// State machine graph. Each node in the graph is a state. Edges are
    /// transitions
    template <class StateType>
    class StateMachine : public StateGraph<StateType> {
    public:
        using EdgeCore = StateMachineEdgeCore;
        using Node = CyclicGraphNode<EdgeCore, StateType>;
        using Base = CyclicGraph<Node>;
        using This = StateMachine<StateType>;
        using InputList = VectorList<String>;
        using NodeSharedPtr = SP<Node>;
        using OnStateChangeFunc = std::function<void(This&)>;

    protected:
        StateType state{};
        StateType prevState{};

        UnorderedMap<StateType, NodeSharedPtr> stateToNodeMap;
        OnStateChangeFunc onStateChangeFunc;

    public:
        /// If true, state transitions can't occur
        bool isLocked = false;

    public:
        /// Map of input to StateType for transitions from any state
        UnorderedMap<String, StateType> anyStateTransitions;

        StateMachine(StateType state = {}) :
            state(state),
            prevState(state) {}

        void SetOnStateChangeFunc(OnStateChangeFunc value) {
            onStateChangeFunc = value;

            // Synchronize state
            GUARD(onStateChangeFunc)
            onStateChangeFunc(*this);
        }

        void OverrideOnStateChangeFunc(OnStateChangeFunc value) {
            SetOnStateChangeFunc(OverrideFunc(onStateChangeFunc, value));
        }

        bool IsLocked() const {
            return isLocked;
        }

        void SetIsLocked(bool value) {
            isLocked = value;
        }

        void Lock() {
            SetIsLocked(true);
        }

        void Unlock() {
            SetIsLocked(false);
        }

        StateType PrevState() const {
            return prevState;
        }

        /// Return a node object for the state (if any)
        NodeSharedPtr NodeForState(StateType state) const {
            auto value = stateToNodeMap.find(state);
            if (value != stateToNodeMap.end()) {
                return value->second;
            }

            return nullptr;
        }

        /// Add a state and a node in the graph
        NodeSharedPtr Add(StateType state) {
            auto node = NodeForState(state);
            if (node) {
                return node;
            }

            node = MAKE<Node>(state);
            this->nodes.insert(node);

            stateToNodeMap[state] = node;

            return node;
        }

        std::optional<StateType> NextStateForInput(String input) {
            // Allow a transition from any state for this input
            try {
                return anyStateTransitions.at(input);
            } catch (...) {}

            auto state = this->state;
            auto node = NodeForState(state);

            GUARDR_LOG(node, {}, "ERROR. State has no node in graph")

            // Search for a matching input
            for (auto& edge : node->Edges()) {
                for (auto& edgeInput : edge->core.inputs) {
                    if (edgeInput == input) {
                        auto toNode = SCAST<Node>(edge->toNode->Value());
                        GUARD_CONTINUE(toNode)
                        return toNode->core;
                    }
                }
            }

            return {};
        }

        void Input(String input) {
            auto nextState = NextStateForInput(input);
            GUARD(nextState)

            // If there is a transition that takes this input from this state, make the transition
            SetState(*nextState);
        }

        /// Connect two states via inputs, adding them to the graph if necessary
        void ConnectStates(StateType fromState, InputList inputs, StateType toState) {
            auto fromNode = Add(fromState);
            auto toNode = Add(toState);

            GUARD(fromNode)
            GUARD(toNode)

            this->AddEdge(fromNode, EdgeCore(inputs), toNode);
        }

        /// Add an input-driven transition between two states
        void AddTransition(StateType fromState, String input, StateType toState) {
            auto fromNode = Add(fromState);
            auto toNode = Add(toState);

            GUARD(fromNode)
            GUARD(toNode)

            this->AddEdge(fromNode, EdgeCore({ input }), toNode);
        }

        StateType State() const {
            return state;
        }

        void SetState(
            StateType value, PublishValueCondition condition = PublishValueCondition::OnChange
        ) {
            auto newState = value;

            switch (condition) {
            case PublishValueCondition::OnChange:
                GUARD(newState != state)
                break;
            default:
                break;
            }

            GUARD(!isLocked)
            GUARD(CanTransition(newState))

            SetStateInternal(newState);
            OnStateChange();
        }

    protected:
        /// Sets state value without broadcasting
        virtual void SetStateInternal(StateType newState) {
            prevState = state;
            state = newState;
        }

    public:
        /// Return true if we can transition to the new state
        virtual bool CanTransition(StateType newState) {
            return true;
        }

        /// Forces the on state change call to run
        /// Used to update state-related properties that have recently changed
        void ForceOnStateChange() {
            OnStateChange();
        }

    protected:
        /// Respond to state change
        virtual void OnStateChange() {
            GUARD(onStateChangeFunc)
            onStateChangeFunc(*this);
        }
    };
} // namespace PJ
