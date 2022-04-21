using System;
using System.Collections;
using NUnit.Framework;

/*
 * RATING: 5 stars. Provides common design patterns for all game objects
 * CODE REVIEW: 4.8.18
 */
namespace PJ {

	class UnitTests_Core { 
		private class TestCore : Core<TestCore.StateType>
		{
			public enum StateType {
				Invalid, On, Off
			}

			public int stateChangedCount;
			public int stateFinishedCount;

			public TestCore() :base()
			{
			}

			protected override void OnStateChange(StateMachine<StateType> inStateMachine)
			{
				base.OnStateChange(inStateMachine);

				stateChangedCount++;
			}

			protected override void OnStateFinish(StateMachine<StateType> inStateMachine)
			{
				base.OnStateFinish(inStateMachine);

				stateFinishedCount++;
			}
		}

		[Test]
		public void UnitTests() {
			var testCore = new TestCore();
			testCore.State = TestCore.StateType.On;
			Assert.AreEqual(1, testCore.stateChangedCount);
			testCore.State = TestCore.StateType.On;
			Assert.AreEqual(1, testCore.stateChangedCount);
			testCore.State = TestCore.StateType.Off;
			Assert.AreEqual(2, testCore.stateChangedCount);

			testCore.StateMachine.SetStateDuration(1.0f);
			testCore.StateMachine.OnUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(1, testCore.stateFinishedCount);
		}
	}
}
