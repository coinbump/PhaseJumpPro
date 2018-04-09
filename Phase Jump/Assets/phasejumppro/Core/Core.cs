using System;
using System.Collections;
using NUnit.Framework;

/*
 * RATING: 5 stars. Provides common design patterns for all game objects
 * CODE REVIEW: 4.8.18
 */
namespace PJ {

	/// <summary>EvtStateChanged
	/// Extends Base by adding some common design patterns
	/// Most game objects have a state machine, so let's go ahead and set that up here
	/// </summary>
	public class Core : Base {
		// OPTIONAL:
		protected Class _class;
		protected string className;	// If class is not yet assigned
		AbstractStateMachine aState;

		public string ClassName {
			get {
				string result = className;
				if (_class != null) {
					result = _class.name;
				}
				return result;
			}
			set {
				className = value;
			}
		}

		public class Listener : PJ.Listener
		{
			WeakReference owner;

			public Listener(Core owner)
			{
				this.owner = new WeakReference(owner);
			}

			public override void EvtListen(Event theEvent)
			{
				if (!owner.IsAlive) { return; }

				if (theEvent.name == EventNames.StateChanged)
				{
					var target = this.owner.Target as Core;
					if (null == target) { return; }
					target.EvtStateChanged(target.aState);
				}
				else if (theEvent.name == EventNames.StateFinished)
				{
					var target = this.owner.Target as Core;
					if (null == target) { return; }
					target.EvtStateFinished();
				}
			}
		}
		public Listener listener;
		public Broadcaster broadcaster = new Broadcaster();

		protected virtual void EvtStateChanged(AbstractStateMachine state) {}
		protected virtual void EvtStateFinished() {}

		public Core()
		{
			listener = new Listener(this);
		}

		public virtual void EvtUpdate(TimeSlice time) {
			if (aState != null) {
				aState.EvtUpdate(time);
			}
		}

		public void SetStateMachine(AbstractStateMachine aState) {
			this.aState = aState;

			if (aState != null) {
				aState.AddListener(listener);
			}
		}

		protected class TestCore : Core
		{
			public enum State {
				Invalid, On, Off
			}

			public class StateMachine : GenericStateMachine<State> {
				
			}
			public StateMachine state = new StateMachine(); 

			public int stateChangedCount;
			public int stateFinishedCount;

			public TestCore() 
			{
				SetStateMachine(this.state);
			}

			protected override void EvtStateChanged(AbstractStateMachine state) {
				base.EvtStateChanged(state);

				stateChangedCount++;
			}

			protected override void EvtStateFinished() {
				base.EvtStateFinished();

				stateFinishedCount++;
			}
		}

		[Test]
		public void UnitTests() {
			var testCore = new TestCore();
			testCore.state.SetState(TestCore.State.On);
			Assert.AreEqual(1, testCore.stateChangedCount);
			testCore.state.SetState(TestCore.State.On);
			Assert.AreEqual(1, testCore.stateChangedCount);
			testCore.state.SetState(TestCore.State.Off);
			Assert.AreEqual(2, testCore.stateChangedCount);

			testCore.state.SetStateDuration(1.0f);
			testCore.state.EvtUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(1, testCore.stateFinishedCount);
		}
	}
}
