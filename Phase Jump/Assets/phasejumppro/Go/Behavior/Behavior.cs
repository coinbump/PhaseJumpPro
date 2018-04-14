using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ {
	/// <summary>
	/// Nodes in a behavior tree allow us to model complex behaviors
	/// </summary>
	public class Behavior : Core {
		public WeakReference parent { get; protected set; }
		protected List<Behavior> children = new List<Behavior>();
		private Behavior _runningChild; // Only root node sets this value

		// Optional:
		public WeakReference owner;

		public Behavior RunningChild {
			get {
				return RootNode()._runningChild;
			}
			set {
				if (null == parent && _runningChild != null && value != _runningChild) {
					Debug.Log("ERROR. Can't run behavior if already running.");
				}
				RootNode()._runningChild = value;
			}
		}

		public enum State {
			Invalid,
			Evalute,		// Run has been called, no result yet
			RunningNode,	// This node is running (set state to Finished when done)
			RunningChild,	// This node is evaluating a child node (sequences, selectors)
			Success,		// Run finished with success
			Fail			// Run finished with failure
		}
		protected GenericStateMachine<State> state = new GenericStateMachine<State>();

		public Behavior()
		{
			SetStateMachine(state);
		}

		public Behavior(WeakReference owner)
		{
			this.owner = owner;
			SetStateMachine(state);
		}

		public void AddChild(Behavior node) {
			node.parent = new WeakReference(this);
			children.Add(node);

			if (node.owner == null) {
				var rootOwner = RootNode().owner;
				node.owner = rootOwner;
			}
		}

		public Behavior RootNode() {
			var result = this;

			var navParent = parent;
			while (navParent != null && navParent.IsAlive) {
				result = navParent.Target as Behavior;
				navParent = result.parent;
			}
			return result;
		}

		protected override void EvtStateChanged(AbstractStateMachine state) {
			if (state != this.state) { return; }

			switch (this.state.state) {
				case State.Fail:
				case State.Success:
					if (null != parent && parent.IsAlive) {
						var parentBehavior = parent.Target as Behavior;
						parentBehavior.EvtChildFinished(this);
					}
					if (this == RunningChild)
					{
						RunningChild = null;
					}
					EvtFinished();
					break;
				case State.RunningNode:
					RunningChild = this;
					break;
			}
		}

		protected virtual void EvtFinished() {}
		protected virtual void EvtChildFinished(Behavior child) {}

		// Call this when you get an Update event
		public override void EvtUpdate(TimeSlice time) {
			base.EvtUpdate(time);

			// Only root node kickstarts Run
			if (null == parent)
			{
				Run();
			}

			// Do this last (Don't Run immediately after child finishes).
			foreach (Behavior child in children) {
				child.EvtUpdate(time);
			}
		}

		public State Run()
		{
			// Can't run if there is a child behavior actively running
			var runningChild = RunningChild;
			if (null != runningChild) { return GetState(); }
			if (IsRunning()) { return GetState(); }

			state.State = State.Evalute;
			_Run();

			switch (state.State) {
				case State.Evalute:
					Debug.Log("ERROR. Behavior node must set state after _Run.");
					break;
			}

			return state.State;
		}

		// Don't call this directly (Run wraps common behavior)
		// Must set the new state via _Run
		protected virtual void _Run() {

			foreach (Behavior child in children) {
				switch (child.Run()) {
					case State.Fail:
						continue;
					default:
						state.State = State.Success;
						return;
				}
			}

			state.State = State.Success;
		}

		public State GetState() { return state.State;  }
		public bool IsRunning() { return state.State == State.RunningNode || state.State == State.RunningChild; }
		public bool IsFinished() { return state.State == State.Success || state.State == State.Fail; }
	}

	public class UnitTests_Behavior {

		private class TestConditionBehavior : ConditionBehavior {
			public bool evaluate;

			public override bool Evaluate()
			{
				return evaluate;
			}
		}

		private class TestTimedBehavior : TimedBehavior {
			public TestTimedBehavior()
				: base(1.0f) {

			}
		}

		[Test]
		public void UnitTests() {
			var t1 = new Behavior(new WeakReference(this));
			var c1 = new TestConditionBehavior();
			var c11 = new TestTimedBehavior();
			var c12 = new Behavior();

			t1.AddChild(c1);
			Assert.AreEqual(c1.owner, t1.owner);

			t1.Run();
			Assert.AreEqual(Behavior.State.Fail, c1.GetState());
			c1.evaluate = true;
			t1.Run();
			Assert.AreEqual(Behavior.State.Success, c1.GetState());

			c1.AddChild(c11);
			c1.AddChild(c12);

			Assert.AreEqual(t1, t1.RootNode());
			Assert.AreEqual(t1, c1.RootNode());
			Assert.AreEqual(t1, c11.RootNode());
			Assert.AreEqual(t1, c12.RootNode());

			t1.Run();
			Assert.AreEqual(t1.RunningChild, c11);
			Assert.IsTrue(c11.IsRunning());
			t1.Run();
			t1.EvtUpdate(new TimeSlice(1.0f));
			Assert.IsTrue(c11.IsFinished());
		}
	}
}
