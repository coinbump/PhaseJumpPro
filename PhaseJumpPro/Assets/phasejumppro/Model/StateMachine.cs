using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 12/11/22
 */
namespace PJ
{
    public partial class StateMachine<T>
    {
        protected T state;
        protected T prevState;

        protected Dictionary<T, Node> stateToNodeMap = new Dictionary<T, Node>();

        /// <summary>
        /// If true, state transitions can't occur
        /// </summary>
        protected bool isLocked;

        /// <summary>
        /// Broadcast state change events
        /// </summary>
        public Broadcaster broadcaster { get; protected set; } = new Broadcaster();

        public bool IsLocked
        {
            get => isLocked;
            set
            {
                isLocked = value;
            }
        }

        public T PrevState => prevState;

        public T State
        {
            get => state;
            set
            {
                var newState = value;
                if (newState.Equals(state))
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
        }

        public void AddListener(SomeListener listener)
        {
            broadcaster.AddListener(listener);
        }

        /// <summary>
        /// Add a state and a node in the graph
        /// </summary>
        public Node AddState(T state)
        {
            var node = NodeForState(state);
            if (null != node)
            {
                return node;
            }

            node = new Node(state);
            nodes.Add(node);

            stateToNodeMap[state] = node;

            return node;
        }

        /// <summary>
        /// Connect two states via inputs, adding them to the graph if necessary
        /// </summary>
        public void ConnectStates(T fromState, List<string> inputs, T toState)
        {
            var fromNode = AddState(fromState);
            var toNode = AddState(toState);

            AddEdge(fromNode, new EdgeModel(inputs), toNode);
        }

        /// <summary>
        /// Return a node object for the state (if any)
        /// </summary>
        public Node NodeForState(T state)
        {
            if (stateToNodeMap.TryGetValue(state, out Node node))
            {
                return node;
            }
            return null;
        }

        public void OnInput(string input)
        {
            var state = this.state;
            var node = NodeForState(state);

            if (null == node)
            {
                Debug.Log("Error. State " + state.ToString() + " has no node in graph");
                return;
            }

            // Search for a matching input. If we find it, do a state transition
            foreach (Node.Edge edge in node.Edges)
            {
                foreach (string edgeInput in edge.model.inputs)
                {
                    if (edgeInput == input)
                    {
                        var toNode = edge.toNode.Value as Node;
                        if (null == toNode) { continue; }

                        State = toNode.state;
                        return;
                    }
                }
            }
        }
    }

    /// <summary>
    /// State machine with state timers, to allow a state to last for N seconds
    /// </summary>
    /// CyclicGraph<GoStateMachine<T>.EdgeModel>
    public partial class StateMachine<T> : CyclicGraph<StateMachine<T>.EdgeModel>, SomeStateMachine<T>
    {
        public struct EdgeModel
        {
            /// <summary>
            /// Inputs that cause a transition to the next state
            /// </summary>
            public List<string> inputs;

            public EdgeModel(List<string> inputs)
            {
                this.inputs = inputs;
            }
        }

        public class Node : CyclicGraphNode<EdgeModel>
        {
            public T state;

            public Node(T state)
            {
                this.state = state;
            }
        }

        public StateMachine()
        {
        }

        public StateMachine(SomeListener listener)
        {
            AddListener(listener);
        }

        /// <summary>
        /// Sets the state value without broadcasting.
        /// </summary>
        /// <param name="newState">New state.</param>
        protected virtual void SetStateInternal(T newState)
        {
            prevState = state;
            state = newState;
        }

        /// <summary>
        /// Return true if we can transition to the new state
        /// </summary>
        public virtual bool CanTransition(T newState) => true;

        /// <summary>
        /// Respond to state change
        /// </summary>
        protected virtual void OnStateChange(T newState)
        {
            broadcaster.Broadcast(new EventStateChange<T>(prevState, newState, this));
        }
    }
}
