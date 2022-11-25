using System;
using System.Collections;

/*
 * RATING: 5 stars
 * Utility class
 * CODE REVIEW: 4/18/22
 */
namespace PJ {

	public interface SomeStateListener<StateType>
	{
		public void OnStateChange(StateMachine<StateType> inStateMachine);
		public void OnStateFinish(StateMachine<StateType> inStateMachine);
	}

	public abstract class SomeCore : Base, Updatable
    {
		public virtual void OnUpdate(TimeSlice time) { }
		public bool IsFinished { get => false; }
	}

	/// <summary>
	/// Provides common design patterns:
	/// 1. Broadcaster, for broadcast-listen events
	/// 2. State machine, for state changes
	/// 3. Class object, for class-type properties shared by multiple objects of the same type
	/// </summary>
	public partial class Core<StateType> : SomeCore {
		// OPTIONAL:
		protected Class _class;
		protected string classId;   // If class is not yet assigned
		protected StateMachine<StateType> stateMachine = new StateMachine<StateType>();

		/// <summary>
		/// If we have a class object, return its id, otherwise use the id we have
		/// </summary>
		public string ClassId {
			get {
				string result = classId;
				if (_class != null) {
					result = _class.id;
				}
				return result;
			}
			set {
				classId = value;
			}
		}

		public StateMachine<StateType> StateMachine => stateMachine;

		public StateType State {
			get => stateMachine.State;
			set {
				stateMachine.State = value;
			}
		}

		public Broadcaster broadcaster = new Broadcaster();

		protected virtual void OnStateChange(StateMachine<StateType> inStateMachine) {}
		protected virtual void OnStateFinish(StateMachine<StateType> inStateMachine) {}

		public Core()
		{
			stateMachine.AddListener(this);
		}

		public override void OnUpdate(TimeSlice time) {
			stateMachine.OnUpdate(time);
		}

		/// <summary>
		/// Allows state events to propagate to a concrete implementation
		/// </summary>
		public void SetStateMachine(StateMachine<StateType> stateMachine)
		{
			if (stateMachine == this.stateMachine) { return; }

			var prevStateMachine = this.stateMachine as SomeBroadcaster;
			if (null != prevStateMachine)
			{
				prevStateMachine.RemoveListener(this);
            }

			this.stateMachine = stateMachine;

			var newStateMachine = stateMachine as SomeBroadcaster;
			if (newStateMachine != null)
			{
				newStateMachine.AddListener(this);
			}
		}
	}

	public partial class Core<StateType> : SomeListener
	{
		public virtual void OnListen(Event theEvent)
		{
			if (!theEvent.sentFrom.TryGetTarget(out object sentFrom)) { return; }

			var stateChangeEvent = theEvent as StateMachine<StateType>.EventStateChange;
			if (null != stateChangeEvent)
			{
				var asStateMachine = sentFrom as StateMachine<StateType>;
				if (null == asStateMachine) { return; }
				OnStateChange(asStateMachine);
				return;
			}

			var stateFinishEvent = theEvent as StateMachine<StateType>.EventStateFinish;
			if (null != stateFinishEvent)
			{
				var asStateMachine = sentFrom as StateMachine<StateType>;
				if (null == asStateMachine) { return; }
				OnStateFinish(asStateMachine);
			}
		}
	}
}
