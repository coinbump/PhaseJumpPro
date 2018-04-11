using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars. Simple design pattern with Unit Tests
 * CODE REVIEW: 4.9.18
 */
namespace PJ {
	/// <summary>
	/// Demo. Stored in a concept. Used for tutorials. Each demo defines behavior.
	/// Go will be called to kickstart the demo. When the demo is finished, it 
	/// should call Finish()
	/// </summary>
	public class Demo : Core {

		public Concept concept;

		protected virtual void EvtFinished() {
			if (null != concept) {
				concept.EvtDemoFinished(this);
			}
		}

		protected override void EvtStateChanged(AbstractStateMachine state) {
			if (state == this.state)
			{
				switch (this.state.state)
				{
					case State.Finished:
						EvtFinished();
						break;
				}
			}
		}

		protected enum State {
			Invalid,
			Ready,
			Finished
		}
		protected GenericStateMachine<State> state = new GenericStateMachine<State>();

		public Demo() {
			SetStateMachine(state);
			state.State = State.Ready;
		}
	
		/// <summary>
		/// Tutorials typically only allow interaction with specific objects.
		/// If tutorial objects have a unique ID, this can check
		/// </summary>
		/// <returns><c>true</c>, if interaction is allowed, <c>false</c> otherwise.</returns>
		/// <param name="componentID">Component identifier.</param>
		public virtual bool CanInteract(string componentID) {
			return true;
		}

		public void Finish() { state.State = State.Finished; }
		public bool IsFinished() { return state.state == State.Finished; }
	}
}
