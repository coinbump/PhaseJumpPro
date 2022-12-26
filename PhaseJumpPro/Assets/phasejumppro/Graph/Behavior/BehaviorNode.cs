using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Nodes in a behavior tree allow us to model complex behaviors
    /// </summary>
    public class BehaviorNode : Graph.GoStandardAcyclicNode, SomeListener
    {
        public enum StateType
        {
            Default,        // Default state
            Evalute,        // Evaluating, no result

            Success,        // Run finished with success
            Fail,           // Run finished with failure

            Running,        // This node is running (VIP: set state to Finish when done)
            Finish          // Was running, now finished
        }

        /// <summary>
        /// Reference to nodes owner (usually the game Node)
        /// </summary>
        public WeakReference<object> owner;

        protected GoStateMachine<StateType> stateMachine = new GoStateMachine<StateType>();

        public BehaviorNode()
        {
            stateMachine.AddListener(this);
        }

        public BehaviorNode(object owner)
        {
            this.owner = new WeakReference<object>(owner);
            stateMachine.AddListener(this);
        }

        public void AddChild(BehaviorNode node)
        {
            AddEdge(new StandardEdgeModel(), node);
            node.owner = owner;
        }

        public override void Go()
        {
            Run();
        }

        public void OnListen(Event theEvent)
        {
            if (!theEvent.sentFrom.TryGetTarget(out object target)) { return; }

            var stateChangeEvent = theEvent as EventStateChange<StateType>;
            if (null != stateChangeEvent)
            {
                OnStateChange(target as SomeStateMachine<StateType>);
            }
        }

        protected virtual void OnStateChange(SomeStateMachine<StateType> stateMachine)
        {
            if (stateMachine != this.stateMachine) { return; }

            switch (this.stateMachine.State)
            {
                // When child stops running, let parent now (will propagate)
                case StateType.Finish:
                    var parent = Parent;
                    if (parent != null)
                    {
                        ((BehaviorNode)parent).stateMachine.State = StateType.Finish;
                    }
                    break;
                default:
                    break;
            }
        }

        /// <summary>
        /// Kickstart the next behavior, unless a node is already running
        /// </summary>
        public virtual StateType Run()
        {
            if (IsRunning) { return StateType.Running; }

            stateMachine.State = StateType.Evalute;
            stateMachine.State = Evaluate();

            return stateMachine.State;
        }

        /// <summary>
        /// Evaluate the behavior
        /// </summary>
        protected virtual StateType Evaluate()
        {
            foreach (Edge edge in edges)
            {
                var child = (BehaviorNode)edge.toNode.Value;
                var childState = child.Run();
                switch (childState)
                {
                    // Fail means this node didn't run and we can pick something else
                    case StateType.Fail:
                        continue;
                    default:
                        return childState;
                }
            }

            return StateType.Fail;
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            // Only root node kickstarts behavior
            if (IsRootNode)
            {
                Run();
            }
        }

        public StateType State => stateMachine.State;

        public bool IsRunning
        {
            get
            {
                return stateMachine.State == StateType.Running;
            }
        }
    }
}
