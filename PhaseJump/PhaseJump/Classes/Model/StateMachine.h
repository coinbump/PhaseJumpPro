#pragma once

#include "Broadcaster.h"
#include "CyclicGraph.h"
#include "Dev.h"
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
    template <class StateType>
    class StateChangeEvent : public Event<StandardEventCore> {
    public:
        using Base = Event<StandardEventCore>;

        StateType prevState;
        StateType state;

        StateChangeEvent(StateType prevState, StateType state, WP<PJ::Base> sentFrom) :
            Base(StandardEventCore(String(""), sentFrom)),
            prevState(prevState),
            state(state) {}
    };

    struct StateMachineEdgeCore {
        using InputList = VectorList<String>;

        /// Inputs that cause a transition to the next state
        InputList inputs;

        StateMachineEdgeCore(InputList inputs) :
            inputs(inputs) {}
    };

    /// State machine graph. Each node in the graph is a state. Edges are
    /// transitions
    template <class StateType>
    class StateMachine : public CyclicGraph<CyclicGraphNode<StateMachineEdgeCore, StateType>>,
                         public SomeStateMachine<StateType> {
    public:
        using EdgeCore = StateMachineEdgeCore;
        using Node = CyclicGraphNode<EdgeCore, StateType>;
        using Base = CyclicGraph<Node>;
        using InputList = VectorList<String>;

        using NodeSharedPtr = SP<Node>;

    protected:
        StateType state = StateType();
        StateType prevState = StateType();

        UnorderedMap<StateType, NodeSharedPtr> stateToNodeMap;

    public:
        /// If true, state transitions can't occur
        bool isLocked = false;

    public:
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

        StateMachine() {}

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

        void OnInput(String input) {
            auto state = this->state;
            auto node = NodeForState(state);

            if (!node) {
                PJ::Log("ERROR. State has no node in graph");
                return;
            }

            // Search for a matching input. If we find it, do a state transition
            for (auto& edge : node->Edges()) {
                for (auto& edgeInput : edge->core.inputs) {
                    if (edgeInput == input) {
                        auto toNode = DCAST<Node>(edge->toNode->Value());
                        if (!toNode) {
                            continue;
                        }

                        SetState(toNode->core);
                        return;
                    }
                }
            }
        }

        /// Connect two states via inputs, adding them to the graph if necessary
        void ConnectStates(StateType fromState, InputList inputs, StateType toState) {
            auto fromNode = AddState(fromState);
            auto toNode = AddState(toState);

            if (nullptr == fromNode || nullptr == toNode) {
                return;
            }

            this->AddEdge(fromNode, EdgeCore(inputs), toNode);
        }

        StateType State() const {
            return state;
        }

        void SetState(StateType value) {
            auto newState = value;
            if (newState == state) {
                return;
            }
            if (isLocked) {
                return;
            }
            if (!CanTransition(newState)) {
                return;
            }

            SetStateInternal(newState);
            OnStateChange(newState);
        }

    protected:
        /// Sets the state value without broadcasting.
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
        virtual void OnStateChange(StateType newState) {}
    };
} // namespace PJ
