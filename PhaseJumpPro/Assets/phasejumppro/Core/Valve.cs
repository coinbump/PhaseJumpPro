using System;
using System.Collections;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/21/22
 */
namespace PJ
{
	public enum InputEffect
	{
		// Change occurs without timer
		Immediate,

		// Change uses timer
		Timed
	}

	/// <summary>
	/// Common design pattern in games. Something has 2 states, and it transitions from state A to state B over N time
	/// Example: a light that turns on and off, or a wheel that turns left and right
	/// </summary>
	public class Valve : PJ.Core<Valve.StateType>
	{
		public enum StateType
		{
			Off,
			TurnOn,	// Valve is turning on (Example: wheel turing to open door)
			On,		// Valve is on (Example: light switch is on)
			TurnOff
		};

		public Optional<float> OnStateDuration {
			get {
				return onStateDuration;
			}
			set {
				onStateDuration = value;

				if (stateMachine.State == StateType.On) { 
					if (onStateDuration == null)
					{
						stateMachine.SetStateDuration(0);
					}
					else
					{
						stateMachine.SetStateDuration(onStateDuration.value);
					}
				}
			}
		}

		public Optional<float> OffStateDuration
		{
			get
			{
				return offStateDuration;
			}
			set
			{
				offStateDuration = value;

				if (stateMachine.State == StateType.Off)
				{
					if (offStateDuration == null)
					{
						stateMachine.SetStateDuration(0);
					}
					else
					{
						stateMachine.SetStateDuration(offStateDuration.value);
					}
				}
			}
		}

		/// <summary>
        /// Timer for turning on transition
        /// </summary>
		public TransformTimer turnOnTimer = new TransformTimer(new InterpolateOutSquared(), 2.0f, SomeTimed.RunType.RunOnce);

		/// <summary>
		/// Timer for turning off transition
		/// </summary>
		public TransformTimer turnOffTimer = new TransformTimer(new InterpolateOutSquared(), 2.0f, SomeTimed.RunType.RunOnce);

		/// <summary>
        /// If true, stops turning animations
        /// </summary>
		public bool isLocked;

		protected float valveState; // (0-1.0) 1.0 is on
		protected Optional<float> onStateDuration = null;    // If not null, hold on state for N seconds, then turn off
		protected Optional<float> offStateDuration = null;   // If not null, hold off state for N seconds, then turn off

		public float ValveState {
			get {
				return valveState;
			}
			set {
				if (!value.Equals(valveState)) {
					valveState = value;
					OnValveStateChange();
				}
			}
		}

		public Valve() {
			SetStateMachine(stateMachine);
		}

		public override void OnUpdate(TimeSlice time) {
			if (isLocked) {
				return;
			}

			switch (stateMachine.State)
			{
				case StateType.TurnOn:
					if (turnOnTimer != null) {
						turnOnTimer.OnUpdate(time);

						ValveState = turnOnTimer.Progress;
						if (turnOnTimer.IsFinished) {
							OnTurnFinish();
						}
					}
					break;
				case StateType.TurnOff:
					if (turnOffTimer != null) {
						turnOffTimer.OnUpdate(time);

						ValveState = 1.0f - turnOffTimer.Progress;
						if (turnOffTimer.IsFinished) {
							OnTurnFinish();
						}
					}
					break;
			}

			// Do this last because it might cause a state change (would cause double time event)
			base.OnUpdate(time);
		}

		protected override void OnStateFinish(StateMachine<StateType> inStateMachine) {
			switch (stateMachine.State)
			{
				case StateType.Off:
					if (turnOnTimer != null) {
						turnOnTimer.Reset();
						stateMachine.State = StateType.TurnOn;
					}
					break;
				case StateType.On:
					if (turnOffTimer != null)
					{
						turnOffTimer.Reset();
						stateMachine.State = StateType.TurnOff;
					}
					break;
			}
		}

		protected override void OnStateChange(StateMachine<StateType> inStateMachine)
		{
			if (inStateMachine != stateMachine) { return; }

			switch (stateMachine.State) {
				case StateType.Off:
					stateMachine.ResetStateTimer();
					ValveState = 0;
					if (offStateDuration != null) {
						stateMachine.SetStateDuration(offStateDuration.value);
					}
					break;
				case StateType.On:
					stateMachine.ResetStateTimer();
					ValveState = 1.0f;
					if (onStateDuration != null)
					{
						stateMachine.SetStateDuration(onStateDuration.value);
					}
					break;
			}
		}

		protected virtual void OnTurnFinish() {
			switch (stateMachine.State) {
				case StateType.TurnOff:
					stateMachine.State = StateType.Off;
					break;
				case StateType.TurnOn:
					stateMachine.State = StateType.On;
					break;
			}
		}

		public void TurnOn(InputEffect inputEffect) {
			var isImmediate = inputEffect == InputEffect.Immediate;
			if (isLocked)
			{
				return;
			}

			if (isImmediate || turnOnTimer == null)
			{
				stateMachine.State = StateType.On;
				return;
			}
			stateMachine.State = StateType.TurnOn;
			turnOnTimer.SetProgress(ValveState);
		}

		public void TurnOff(InputEffect inputEffect) {
			var isImmediate = inputEffect == InputEffect.Immediate;
			if (isLocked)
			{
				return;
			}

			if (isImmediate || turnOffTimer == null) {
				stateMachine.State = StateType.Off;
				return;
			}
			stateMachine.State = StateType.TurnOff;
			turnOffTimer.SetProgress(1.0f - ValveState);
		}

		public void Switch(InputEffect inputEffect) {
			if (isLocked)
			{
				return;
			}

			switch (stateMachine.State) {
				case StateType.Off:
				case StateType.TurnOff:
					TurnOn(inputEffect);
					break;
				case StateType.On:
				case StateType.TurnOn:
					TurnOff(inputEffect);
					break;
			}
		}

		public void InputBinary(bool binary, InputEffect inputEffect) {
			if (binary) {
				TurnOn(inputEffect);
			}
			else {
				TurnOff(inputEffect);
			}
		}

		// OPTIONAL:
		virtual protected void OnValveStateChange() { }
		virtual protected void OnOn() { }
		virtual protected void OnOff() { }
	}
}
