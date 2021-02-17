using System;
using System.Collections;

/*
 * RATING: 4 stars. Has Unit Tests, but could use more
 * CODE REVIEW: 4.8.18
 */
namespace PJ
{
	public enum InputEffect
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
		protected StateMachine<State> state;
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
			state = new StateMachine<State>();
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

		protected override void EvtStateFinished(SomeStateMachine state) {
			if (isLocked)
			{
				return;
			}

			switch (this.state.state)
			{
				case State.Off:
					if (turnOnTimer != null) {
						turnOnTimer.Reset();
						this.state.State = State.TurnOn;
					}
					break;
				case State.On:
					if (turnOffTimer != null)
					{
						turnOffTimer.Reset();
						this.state.State = State.TurnOff;
					}
					break;
			}
		}

		protected override void EvtStateChanged(SomeStateMachine state)
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
						TurnOn(InputEffect.OverTime);
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
						TurnOff(InputEffect.OverTime);
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
					state.State = State.Off;
					break;
				case State.TurnOn:
					state.State = State.On;
					break;
			}
		}

		public void TurnOn(InputEffect input) {
			var immediate = input == InputEffect.Immediate;
			if (isLocked)
			{
				return;
			}

			if (immediate || turnOnTimer == null)
			{
				state.State = State.On;
				return;
			}
			state.State = State.TurnOn;
			turnOnTimer.SetProgress(ValveState);
		}

		public void TurnOff(InputEffect input) {
			var immediate = input == InputEffect.Immediate;
			if (isLocked)
			{
				return;
			}

			if (immediate || turnOffTimer == null) {
				state.State = State.Off;
				return;
			}
			state.State = State.TurnOff;
			turnOffTimer.SetProgress(1.0f-ValveState);
		}

		public void Switch(InputEffect input) {
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

		public void InputBinary(bool binary, InputEffect input) {
			if (binary) {
				TurnOn(input);
			}
			else {
				TurnOff(input);
			}
		}

		public State GetState() {
			return state.state;
		}

		// OPTIONAL:
		virtual protected void EvtValveStateChanged() { }
		virtual protected void EvtOn() { }
		virtual protected void EvtOff() { }
	}
}
