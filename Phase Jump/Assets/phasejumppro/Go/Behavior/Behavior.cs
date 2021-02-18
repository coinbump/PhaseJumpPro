using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 4 stars. Has Unit Tests, could use visual tests
 * CODE REVIEW: 2.17.21
 */
namespace PJ
{
	/// <summary>
	/// Nodes in a behavior tree allow us to model complex behaviors
	/// </summary>
	public class Behavior : Core
	{
		public WeakReference parent { get; protected set; }
		protected List<Behavior> children = new List<Behavior>();

		// Optional: useful to store a game object to alter its behavior
		public WeakReference owner;

		public Behavior RunningNode
		{
			get
			{
				var rootNode = RootNode();
				return rootNode.runStack.Count == 0 ? null : rootNode.runStack.Peek();
			}
		}

		public enum State
		{
			Default,        // Default state
			Evalute,        // Run has been called, no result yet
			Running,        // This node is running (set state to Success/Fail when done)
			Success,        // Run finished with success
			Fail            // Run finished with failure
		}
		protected StateMachine<State> state = new StateMachine<State>();

		public Behavior()
		{
			SetStateMachine(state);
		}

		public Behavior(WeakReference owner)
		{
			this.owner = owner;
			SetStateMachine(state);
		}

		public void AddChild(Behavior node)
		{
			node.parent = new WeakReference(this);
			children.Add(node);

			if (node.owner == null)
			{
				var rootOwner = RootNode().owner;
				node.owner = rootOwner;
			}
		}

		public RootBehavior RootNode()
		{
			Behavior result = this;

			var navParent = parent;
			while (navParent != null && navParent.IsAlive)
			{
				result = navParent.Target as Behavior;
				navParent = result.parent;
			}

			Debug.Assert(result is RootBehavior, "Root of behavior tree must be RootBehavior");
			return result as RootBehavior;
		}

		protected override void EvtStateChanged(SomeStateMachine state)
		{
			if (state != this.state) { return; }

			switch (this.state.state)
			{
				case State.Fail:
				case State.Success:
					var isRunningNode = this == RunningNode;
					if (isRunningNode)
					{
						var runStack = RootNode().runStack;
						runStack.Pop();
					}
					if (null != parent && parent.IsAlive)
					{
						var parentBehavior = parent.Target as Behavior;
						parentBehavior.EvtChildFinished(this, isRunningNode);
					}
					EvtFinished();
					break;
				case State.Running:
					if (RunningNode != this)
					{
						RootNode().runStack.Push(this);
					}
					break;
			}
		}

		protected virtual void EvtFinished() { }
		protected virtual void EvtChildFinished(Behavior child, bool wasRunningNode) { }

		// Call this when you get an Update event
		public override void EvtUpdate(TimeSlice time)
		{
			base.EvtUpdate(time);

			// Only root node kickstarts Run
			if (null == parent)
			{
				// Can't run if there is a node actively running
				if (null == RunningNode)
				{
					Run();
				}
			}

			// Do this last (Don't Run immediately after child finishes).
			foreach (Behavior child in children)
			{
				child.EvtUpdate(time);
			}
		}

		public State Run()
		{
			state.State = State.Evalute;
			state.State = Evaluate();

			switch (state.State)
			{
				case State.Evalute:
					Debug.Log("ERROR. Behavior node must set state after Evaluate.");
					break;
			}

			return state.State;
		}

		// Don't call this directly (called by Run)
		// Must set the new state via _Run
		/// <summary>
		/// Evaluate the behavior
		/// </summary>
		protected virtual State Evaluate()
		{
			foreach (Behavior child in children)
			{
				var childState = child.Run();
				switch (childState)
				{
					case State.Fail:
						continue;
					default:
						// If child is success or running, it evaluated, so quit looking for a node
						return State.Success;
				}
			}

			return State.Success;
		}

		public State GetState() { return state.State; }
		public bool IsRunning() { return state.State == State.Running; }
		public bool IsFinished() { return state.State == State.Success || state.State == State.Fail; }
	}

	/// <summary>
	/// Holds run stack for entire behavior tree
	/// </summary>
	public class RootBehavior : Behavior
	{
		public Stack<Behavior> runStack = new Stack<Behavior>();

		public RootBehavior()
			:	base()
        {

        }

		public RootBehavior(WeakReference owner)
			: base(owner)
		{

		}
	}
}
