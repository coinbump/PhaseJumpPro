using System;
using System.Collections;
using NUnit.Framework;

/*
 * RATING: 5 stars. Provides common design patterns for all game objects
 * CODE REVIEW: 4.8.18
 */
namespace PJ {

	class UnitTests_Core { 
		private class TestCore : Core
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
			testCore.state.State = TestCore.State.On;
			Assert.AreEqual(1, testCore.stateChangedCount);
			testCore.state.State = TestCore.State.On;
			Assert.AreEqual(1, testCore.stateChangedCount);
			testCore.state.State = TestCore.State.Off;
			Assert.AreEqual(2, testCore.stateChangedCount);

			testCore.state.SetStateDuration(1.0f);
			testCore.state.EvtUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(1, testCore.stateFinishedCount);
		}
	}
}
