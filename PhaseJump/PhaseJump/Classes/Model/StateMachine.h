#ifndef PJSTATEMACHINE_H
#define PJSTATEMACHINE_H

#include "CyclicGraph.h"
#include "SomeStateMachine.h"
#include "Broadcaster.h"
#include "_Map.h"
#include "_Set.h"
#include "List.h"
#include <iostream>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/11/22
 */
namespace PJ
{
    template <class T>
    class EventStateChange : public Event
    {
    public:
        T prevState;
        T state;

        EventStateChange(T prevState, T state, std::weak_ptr<PJ::Base> sentFrom) :
        Event(String(""), sentFrom),
        prevState(prevState),
        state(state)
        {
        }
    };

    struct StateMachineEdgeModel
    {
        using InputList = List<String>;

        /// <summary>
        /// Inputs that cause a transition to the next state
        /// </summary>
        InputList inputs;

        StateMachineEdgeModel(InputList inputs) : inputs(inputs)
        {
        }
    };

    /// <summary>
    /// State machine graph. Each node in the graph is a state. Edges are transitions
    /// </summary>
    template <class T>
    class StateMachine : public CyclicGraph<StateMachineEdgeModel>, public SomeStateMachine<T>
    {
    public:
        using EdgeModel = StateMachineEdgeModel;
        using Base = CyclicGraph<StateMachineEdgeModel>;
        using InputList = List<String>;

        class Node : public CyclicGraphNode<EdgeModel>
        {
        public:
            T state;

            Node(T state) : state(state)
            {
            }
        };

        using NodeSharedPtr = std::shared_ptr<Node>;

    protected:
        T state;
        T prevState;

        Map<T, NodeSharedPtr> stateToNodeMap;

    public:
        /// <summary>
        /// Broadcast state change events
        /// </summary>
        std::shared_ptr<Broadcaster> broadcaster = std::make_shared<Broadcaster>();

        /// <summary>
        /// If true, state transitions can't occur
        /// </summary>
        bool isLocked;

    public:
        bool IsLocked() const { return isLocked; }
        void SetIsLocked(bool value)
        {
            isLocked = value;
        }

        T PrevState() const { return prevState; }

        StateMachine()
        {
        }

        StateMachine(Broadcaster::ListenerWeakPtr listener)
        {
            AddListener(listener);
        }

        /// <summary>
        /// Return a node object for the state (if any)
        /// </summary>
        NodeSharedPtr NodeForState(T state) const
        {
            auto value = stateToNodeMap.find(state);
            if (value != stateToNodeMap.end()) {
                return value->second;
            }

            return NodeSharedPtr();
        }

        /// <summary>
        /// Add a state and a node in the graph
        /// </summary>
        NodeSharedPtr AddState(T state)
        {
            auto node = NodeForState(state);
            if (node)
            {
                return node;
            }

            node = std::make_shared<Node>(state);
            this->nodes.Add(node);

            stateToNodeMap[state] = node;

            return node;
        }

        void OnInput(String input)
        {
            auto state = this->state;
            auto node = NodeForState(state);

            if (!node)
            {
                std::cout << "ERROR. State has no node in graph";
                return;
            }

            // Search for a matching input. If we find it, do a state transition
            for (auto edge : node->Edges())
            {
                for (auto edgeInput : edge->model.inputs)
                {
                    if (edgeInput == input)
                    {
                        auto toNode = std::dynamic_pointer_cast<Node>(edge->toNode->Value());
                        if (!toNode) { continue; }

                        SetState(toNode->state);
                        return;
                    }
                }
            }
        }

        /// <summary>
        /// Connect two states via inputs, adding them to the graph if necessary
        /// </summary>
        void ConnectStates(T fromState, InputList inputs, T toState)
        {
            auto fromNode = AddState(fromState);
            auto toNode = AddState(toState);

            if (nullptr == fromNode || nullptr == toNode) { return; }

            AddEdge(fromNode, EdgeModel(inputs), toNode);
        }

        void AddListener(Broadcaster::ListenerWeakPtr listener)
        {
            broadcaster->AddListener(listener);
        }

        T State() const { return state; }
        void SetState(T value)
        {
            auto newState = value;
            if (newState == state)
            {
                return;
            }
            if (isLocked)
            {
                return;
            }
            if (!CanTransition(newState))
            {
                return;
            }

            SetStateInternal(newState);
            OnStateChange(newState);
        }

    protected:
        /// <summary>
        /// Sets the state value without broadcasting.
        /// </summary>
        virtual void SetStateInternal(T newState)
        {
            prevState = state;
            state = newState;
        }

    public:
        /// <summary>
        /// Return true if we can transition to the new state
        /// </summary>
        virtual bool CanTransition(T newState) { return true; }

    protected:
        /// <summary>
        /// Respond to state change
        /// </summary>
        virtual void OnStateChange(T newState)
        {
            broadcaster->Broadcast(std::make_shared<EventStateChange<T>>(prevState, newState, this->shared_from_this()));
        }
    };
}

#endif
