using System;
using System.Collections;
using NUnit.Framework;

/*
 * RATING: 4 stars. Has Unit Tests, but could use more
 * CODE REVIEW: 4.8.18
 */
namespace PJ
{
	public enum Input
	{
		Immediate, OverTime
	}

	/// <summary>
	/// Common design pattern in games. Something has 2 states, and it transitions from state A to state B over N time
	/// Example: a light that turns on and off, or a wheel that turns left and right
	/// </summary>
	public class Valve : Core
	{
		public enum State
		{
			Invalid,
			Off,
			TurnOn,
			On,
			TurnOff
		};
		private float _timeOn = -1.0f;    // > 0: switch after N seconds. < 0: lock in place
		private float _timeOff = -1.0f;   // > 0: switch after N seconds. < 0: lock in place

		public float TimeOn {
			get {
				return _timeOn;
			}
			set {
				if (!_timeOn.Equals(value))
				{
					_timeOn = value;

					if (state.state == State.On) { 
						state.ResetStateTimer();

						if (_timeOn < 0)
						{
							state.SetStateDuration(0);
						}
						else
						{
							state.SetStateDuration(_timeOn);
						}
					}
				}
			}
		}

		public float TimeOff
		{
			get
			{
				return _timeOff;
			}
			set
			{
				if (!_timeOff.Equals(value))
				{
					_timeOff = value;

					if (state.state == State.Off)
					{
						state.ResetStateTimer();

						if (_timeOff < 0)
						{
							state.SetStateDuration(0);
						}
						else
						{
							state.SetStateDuration(_timeOff);
						}
					}
				}
			}
		}

		public InterpolateTimer turnOnTimer;
		public InterpolateTimer turnOffTimer;
		public GenericStateMachine<State> state { get; protected set; }
		public bool isLocked;
		private float _valveState; // 1.0 is on

		public float ValveState {
			get {
				return _valveState;
			}
			set {
				if (!value.Equals(_valveState)) {
					_valveState = value;
					EvtValveStateChanged();
				}
			}
		}

		public Valve() {
			state = new GenericStateMachine<State>();
			SetStateMachine(state);
		}

		public override void EvtUpdate(TimeSlice time) {
			if (isLocked) {
				return;
			}
			switch (state.state)
			{
				case State.TurnOn:
					if (turnOnTimer != null) {
						turnOnTimer.EvtUpdate(time);

						ValveState = turnOnTimer.GetProgress();
						if (turnOnTimer.IsFinished) {
							EvtFinished();
						}
					}
					break;
				case State.TurnOff:
					if (turnOffTimer != null) {
						turnOffTimer.EvtUpdate(time);

						ValveState = 1.0f-turnOffTimer.GetProgress();
						if (turnOffTimer.IsFinished) {
							EvtFinished();
						}
					}
					break;
			}

			// Do this last because it might cause a state change (would cause double time event)
			base.EvtUpdate(time);
		}

		protected override void EvtStateFinished() {
			if (isLocked)
			{
				return;
			}

			switch (state.state)
			{
				case State.Off:
					if (turnOnTimer != null) {
						turnOnTimer.Reset();
						state.SetState(State.TurnOn);
					}
					break;
				case State.On:
					if (turnOffTimer != null)
					{
						turnOffTimer.Reset();
						state.SetState(State.TurnOff);
					}
					break;
			}
		}

		protected override void EvtStateChanged(AbstractStateMachine state)
		{
			if (state != this.state) { return; }

			switch (this.state.state) {
				case State.Off:
					this.state.ResetStateTimer();
					ValveState = 0;
					if (TimeOff > 0) {
						this.state.SetStateDuration(TimeOff);
					}
					else if (TimeOff.Equals(0)) {
						this.state.SetStateDuration(0);
						TurnOn(Input.OverTime);
					}
					break;
				case State.On:
					this.state.ResetStateTimer();
					ValveState = 1.0f;
					if (TimeOn > 0) {
						this.state.SetStateDuration(TimeOn);
					}
					else if (TimeOn.Equals(0)) {
						this.state.SetStateDuration(0);
						TurnOff(Input.OverTime);
					}
					break;
			}
		}

		protected virtual void EvtFinished() {
			if (isLocked) {
				return;
			}

			switch (state.state) {
				case State.TurnOff:
					state.SetState(State.Off);
					break;
				case State.TurnOn:
					state.SetState(State.On);
					break;
			}
		}

		public void TurnOn(Input input) {
			var immediate = input == Input.Immediate;
			if (isLocked)
			{
				return;
			}

			if (immediate || turnOnTimer == null)
			{
				state.SetState(State.On);
				return;
			}
			state.SetState(State.TurnOn);
			turnOnTimer.SetProgress(ValveState);
		}

		public void TurnOff(Input input) {
			var immediate = input == Input.Immediate;
			if (isLocked)
			{
				return;
			}

			if (immediate || turnOffTimer == null) {
				state.SetState(State.Off);
				return;
			}
			state.SetState(State.TurnOff);
			turnOffTimer.SetProgress(1.0f-ValveState);
		}

		public void Switch(Input input) {
			if (isLocked)
			{
				return;
			}

			switch (state.state) {
				case State.Off:
				case State.TurnOff:
					TurnOn(input);
					break;
				case State.On:
				case State.TurnOn:
					TurnOff(input);
					break;
			}
		}

		public void InputBinary(bool binary, Input input) {
			if (binary) {
				TurnOn(input);
			}
			else {
				TurnOff(input);
			}
		}

		// OPTIONAL:
		virtual protected void EvtValveStateChanged() { }
		virtual protected void EvtOn() { }
		virtual protected void EvtOff() { }
	}

	class UnitTests_Valve {
		class TestValve : Valve {
			public TestValve() {
				turnOnTimer = new InterpolateTimer(new InterpolateLinear(), AbstractTimed.Type.Persistent, 1.0f);
				turnOffTimer = new InterpolateTimer(new InterpolateLinear(), AbstractTimed.Type.Persistent, 1.0f);
				TimeOn = 1.0f;
				TimeOff = 1.0f;
			}
		}

		[Test]
		public void UnitTests() {
			var test = new TestValve();
			test.EvtUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(Valve.State.Invalid, test.state.state);
			test.state.SetState(Valve.State.Off);
			Assert.AreEqual(0, test.ValveState);

			test.EvtUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(Valve.State.TurnOn, test.state.state);

			test.EvtUpdate(new TimeSlice(0.5f));
			Assert.AreEqual(.5f, test.ValveState);
			test.EvtUpdate(new TimeSlice(0.5f));
			Assert.AreEqual(Valve.State.On, test.state.state);

			test.EvtUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(Valve.State.TurnOff, test.state.state);

			test.EvtUpdate(new TimeSlice(0.5f));
			Assert.AreEqual(.5f, test.ValveState);
			test.EvtUpdate(new TimeSlice(0.5f));
			Assert.AreEqual(Valve.State.Off, test.state.state);

			test.TimeOff = -1.0f;
			test.EvtUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(Valve.State.Off, test.state.state);

			test.TimeOff = 1.0f;
			test.TurnOn(Input.OverTime);
			test.EvtUpdate(new TimeSlice(.5f));
			Assert.AreEqual(Valve.State.TurnOn, test.state.state);
			test.EvtUpdate(new TimeSlice(.5f));
			Assert.AreEqual(Valve.State.On, test.state.state);

			test.TurnOff(Input.Immediate);
			Assert.AreEqual(Valve.State.Off, test.state.state);
			Assert.AreEqual(0, test.ValveState);
		}
	}
}
