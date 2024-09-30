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
        NodeSharedPtr AddState(StateType state) {
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
            auto fromNode = AddState(fromState);
            auto toNode = AddState(toState);

            GUARD(fromNode)
            GUARD(toNode)

            this->AddEdge(fromNode, EdgeCore(inputs), toNode);
        }

        StateType State() const {
            return state;
        }

        void SetState(StateType value) {
            auto newState = value;

            GUARD(newState != state)
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

    protected:
        /// Respond to state change
        virtual void OnStateChange() {
            GUARD(onStateChangeFunc)
            onStateChangeFunc(*this);
        }
    };
} // namespace PJ
