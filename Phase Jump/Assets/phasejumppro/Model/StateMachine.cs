using System;
using NUnit.Framework;

/*
 * RATING: 5 stars. Core functionality with Unit Tests
 * CODE REVIEW: 3.18.18
 */
namespace PJ {

	/// <summary>
	/// Abstract state machine.
	/// </summary>
	public abstract class AbstractStateMachine {
		// FUTURE: support class objects if needed (see PJ C++)
		//public class Class : PJ.Class {
		//}

    	//// OPTIONAL:
        //public StateMachineClass _class;
    }

	/// <summary>
	/// Generic state machine (typically T is an enum).
	/// </summary>
	public class GenericStateMachine <T> : AbstractStateMachine where T : struct, IConvertible
    {
		// USAGE: best practice: the T type should have a default value that represents an invalid state
		public T state { get; protected set; }
		public T prevState { get; protected set; }

		// OPTIONAL:
    	protected float stateDuration;
    	protected float stateReverseTimer;
    	protected bool stateLocked;
		public float timeInState { get; protected set; }

    	public void Lock(bool lockState) {
            stateLocked = lockState;
    	}
    	
        public void SetStateDuration(float duration) {
    		stateDuration = duration;
			stateReverseTimer = duration;
    	}

    	public void ResetStateTimer() {
    		stateReverseTimer = stateDuration;
    	}

        public void CancelStateTimer() {
            stateReverseTimer = 0; // Keep duration if we need to reset the timer later.
    	}
			
    	public virtual bool SetState(T newState)
        {
            if (newState.Equals(state)) {
    			return true;
    		}
    		if (stateLocked) {
    			return false;
    		}
    		if (!CanTransition(newState)) {
    			return false;
    		}

    		SetStateVal(newState);
    		EvtStateChanged(newState);

    		return true;
    	}

        /// <summary>
        /// Sets the state value without broadcasting.
        /// </summary>
        /// <param name="newState">New state.</param>
    	public void SetStateVal(T newState)
    	{
            if (newState.Equals(state)) {
    			return;
    		}
    		prevState = state;
    		state = newState;
    		CancelStateTimer(); // State duration is no longer valid for new state.
    	}
			
    	public virtual float GetProgress() {
    		if (0 == stateDuration) {
    			return 0;
    		}

    		float result = 1.0f - (stateReverseTimer / stateDuration);

    		// Cap values to avoid weirdness in case of sudden time jumps.
            result = Math.Max(0, Math.Min(1.0f, result));
    		return result;
    	}
			
    	// Override to handle logic of a state transition
		public virtual bool CanTransition(T newState) {
    		return true; // TRUE: go ahead and change states
    	}

	  	// Override to respond to state changes.
    	protected virtual void EvtStateChanged(T newState)
    	{
            timeInState = 0;

            // FUTURE: Add broadcast-listener support as needed
    		//Broadcast(tEventType.kEvtStatePreChange); // Respond before any action is taken
    		//Broadcast(tEventType.kEvtStateChanged);
    	}

		protected virtual void EvtStateFinished()
		{
			// FUTURE: Add broadcast-listener support as needed
    		//Broadcast(tEventType.kEvtStateFinished);
    	}

    	public virtual void EvtUpdate(TimeSlice time)
    	{
    		if (stateReverseTimer > 0) {
				stateReverseTimer -= time.delta;
    			if (stateReverseTimer <= 0) {
    				EvtStateFinished();
    			}
    		}
			timeInState += time.delta;
    	}

	}

	/// <summary>
	/// Unit Tests
	/// </summary>
	public class StateMachine_UnitTests
	{
		enum TestEnum
		{
			Invalid,
			Test1,
			Test2
		}

		class TestStateMachine : GenericStateMachine<TestEnum>
		{
			public int test1Count { get; protected set; }
			public int test2Count { get; protected set; }
			public int finishedCount { get; protected set; }

			protected override void EvtStateFinished()
			{
				base.EvtStateFinished();

				finishedCount++;
			}

			protected override void EvtStateChanged(TestEnum newState)
			{
				base.EvtStateChanged(newState);

				switch (newState) {
					case TestEnum.Test1:
						test1Count++;
						break;
					case TestEnum.Test2:
						test2Count++;
						break;
				}
			}
		}

		[Test]
		public void UnitTests()
		{
			var test = new TestStateMachine();
			test.SetState(TestEnum.Test2);
			Assert.AreEqual(1, test.test2Count);
			test.Lock(true);

			test.SetState(TestEnum.Test1);
			Assert.AreEqual(0, test.test1Count);
			test.Lock(false);
			test.SetState(TestEnum.Test1);
			Assert.AreEqual(1, test.test1Count);

			test.SetStateDuration(1.0f);
			test.EvtUpdate(new TimeSlice(1.5f));
			Assert.AreEqual(test.finishedCount, 1);
			test.ResetStateTimer();
			test.EvtUpdate(new TimeSlice(.5f));
			Assert.AreEqual(.5f, test.GetProgress(), .001f);
			test.EvtUpdate(new TimeSlice(.6f));
			Assert.AreEqual(test.finishedCount, 2);

			test.SetState(TestEnum.Test2);
			Assert.AreEqual(TestEnum.Test1, test.prevState);
		}

    }
       
}

