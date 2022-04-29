using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/20/22
 */
namespace PJ
{
	/// <summary>
	/// State machine with state timers, to allow a state to last for N seconds
	/// </summary>
	public partial class StateMachine<T> : Graph.CyclicGraph<StateMachine<T>.EdgeModel>, SomeStateMachine, Updatable
	{
		/// <summary>
        /// Modifiers to modify the standard state behavior
        /// </summary>
		public HashSet<SomeModifier> modifiers = new HashSet<SomeModifier>();

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

		public class Node : Graph.CyclicNode<EdgeModel>
		{
			public T state;

			public Node(T state)
            {
				this.state = state;
            }
		}

		protected T state;
		protected T prevState;

		protected Dictionary<T, Node> stateToNodeMap = new Dictionary<T, Node>();

		/// <summary>
		/// Broadcast state change events
		/// </summary>
		public Broadcaster broadcaster { get; protected set; } = new Broadcaster();

		/// <summary>
        /// Time in the current state since the last state transition (requires OnUpdate)
        /// </summary>
		public float timeInState { get; protected set; }

		// OPTIONAL:
		/// <summary>
        /// Time to spend in current state before notification
        /// </summary>
		protected float stateDuration;

		/// <summary>
        /// Countdown timer for state duration (starts from duration, counts to zero)
        /// </summary>
		protected float stateCountdownTimer;

		/// <summary>
        /// If true, state transitions can't occur
        /// </summary>
		protected bool isLocked;

		public bool IsLocked
        {
			get => isLocked;
			set
            {
				isLocked = value;
            }
        }

		public T PrevState => prevState;

		public StateMachine()
        {
        }

		public StateMachine(SomeListener listener)
        {
			AddListener(listener);
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

		public void OnInput(string input)
        {
			var state = this.state;
			var node = NodeForState(state);

			if (null == node) {
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

		/// <summary>
        /// Connect two states via inputs, adding them to the graph if necessary
        /// </summary>
		public void ConnectStates(T fromState, List<string> inputs, T toState)
		{
			var fromNode = AddState(fromState);
			var toNode = AddState(toState);

			AddEdge(fromNode, new EdgeModel(inputs), toNode);
		}

		public void AddListener(SomeListener listener)
		{
			broadcaster.AddListener(listener);
		}

		public void SetStateDuration(float duration)
		{
			stateDuration = duration;
			stateCountdownTimer = duration;
		}

		public void ResetStateTimer()
		{
			stateCountdownTimer = stateDuration;
		}

		public void CancelStateTimer()
		{
			stateCountdownTimer = 0; // Keep duration if we need to reset the timer later.
		}

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

		/// <summary>
		/// Sets the state value without broadcasting.
		/// </summary>
		/// <param name="newState">New state.</param>
		protected void SetStateInternal(T newState)
		{
			prevState = state;
			state = newState;
			CancelStateTimer(); // State duration is no longer valid for new state.
		}

		public float StateProgress
		{
			get
			{
				if (stateDuration <= 0)
				{
					return 0;
				}

				float result = 1.0f - (stateCountdownTimer / stateDuration);

				result = Math.Max(0, Math.Min(1.0f, result));
				return result;
			}
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
			timeInState = 0;
			broadcaster.Broadcast(new EventStateChange(prevState, state, this));
		}

		protected virtual void OnStateFinish()
		{
			broadcaster.Broadcast(new EventStateFinish(state, this));
		}

		public virtual void OnUpdate(TimeSlice time)
		{
			var iterModifiers = new HashSet<SomeModifier>(modifiers);
			foreach (SomeModifier modifier in iterModifiers) {
				if (!modifier.IsFinished)
				{
					modifier.OnUpdate(time);
				}

				if (modifier.IsFinished)
                {
					switch (modifier.runType)
                    {
						case SomeTimed.RunType.RunOnce:
							modifiers.Remove(modifier);
							break;
						case SomeTimed.RunType.KeepRunning:
							modifier.Reset();
							break;
                    }
                }
			}

			if (stateCountdownTimer > 0)
			{
				stateCountdownTimer -= time.delta;
				if (stateCountdownTimer <= 0)
				{
					OnStateFinish();
				}
			}
			timeInState += time.delta;
		}
	}
}

