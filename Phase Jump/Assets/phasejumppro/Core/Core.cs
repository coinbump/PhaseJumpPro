using System;
using System.Collections;

/*
 * RATING: 5 stars. Provides common design patterns for all game objects
 * CODE REVIEW: 4.8.18
 */
namespace PJ {

	/// <summary>
	/// Extends Base by adding some common design patterns
	/// Most game objects have a state machine, so let's go ahead and set that up here
	/// </summary>
	public class Core : Base {
		// OPTIONAL:
		protected Class _class;
		protected string classIdentifier;	// If class is not yet assigned
		SomeStateMachine someState;

		public string ClassIdentifier {
			get {
				string result = classIdentifier;
				if (_class != null) {
					result = _class.Identifier;
				}
				return result;
			}
			set {
				classIdentifier = value;
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
				bool isSentFromAlive = null != theEvent.sentFrom && theEvent.sentFrom.IsAlive;

				if (theEvent.name == EventNames.StateChanged)
				{
					var target = this.owner.Target as Core;
					if (null == target) { return; }
					if (!isSentFromAlive) { return; }

					target.EvtStateChanged(theEvent.sentFrom.Target as SomeStateMachine);
				}
				else if (theEvent.name == EventNames.StateFinished)
				{
					var target = this.owner.Target as Core;
					if (null == target) { return; }
					if (!isSentFromAlive) { return; }

					target.EvtStateFinished(theEvent.sentFrom.Target as SomeStateMachine);
				}
			}
		}
		public Listener listener;
		public Broadcaster broadcaster = new Broadcaster();

		protected virtual void EvtStateChanged(SomeStateMachine state) {}
		protected virtual void EvtStateFinished(SomeStateMachine state) {}

		public Core()
		{
			listener = new Listener(this);
		}

		public virtual void EvtUpdate(TimeSlice time) {
			// A state machine may need time for timed states
			if (someState != null) {
				someState.EvtUpdate(time);
			}
		}

		/// <summary>
		/// Allows state events to propagate to a concrete implementation
		/// </summary>
		public void SetStateMachine(SomeStateMachine someState)
		{
			this.someState = someState;

			if (someState != null)
			{
				someState.AddListener(listener);
			}
		}
	}
}
