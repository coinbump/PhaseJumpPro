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
				bool isSentFromAlive = null != theEvent.sentFrom && theEvent.sentFrom.IsAlive;

				if (theEvent.name == EventNames.StateChanged)
				{
					var target = this.owner.Target as Core;
					if (null == target) { return; }
					if (!isSentFromAlive) { return; }

					target.EvtStateChanged(theEvent.sentFrom.Target as AbstractStateMachine);
				}
				else if (theEvent.name == EventNames.StateFinished)
				{
					var target = this.owner.Target as Core;
					if (null == target) { return; }
					if (!isSentFromAlive) { return; }

					target.EvtStateFinished(theEvent.sentFrom.Target as AbstractStateMachine);
				}
			}
		}
		public Listener listener;
		public Broadcaster broadcaster = new Broadcaster();

		protected virtual void EvtStateChanged(AbstractStateMachine state) {}
		protected virtual void EvtStateFinished(AbstractStateMachine state) {}

		public Core()
		{
			listener = new Listener(this);
		}

		public virtual void EvtUpdate(TimeSlice time) {
			// A state machine may need time for timed states
			if (aState != null) {
				aState.EvtUpdate(time);
			}
		}

		/// <summary>
		/// Allows state events to propagate to a concrete implementation
		/// </summary>
		public void SetStateMachine(AbstractStateMachine aState)
		{
			this.aState = aState;

			if (aState != null)
			{
				aState.AddListener(listener);
			}
		}
	}
}
