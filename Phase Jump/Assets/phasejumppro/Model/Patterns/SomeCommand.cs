using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars. Simple design pattern with Unit Tests
 * CODE REVIEW: 4.7.18
 */
namespace PJ
{
	/// <summary>
	/// Commands are reversible. Typically a command will store a Memento that allows it to reverse
	/// the state
	/// </summary>
	public abstract class SomeCommand
	{
		public enum State {
			Default,
			Complete,
			Reversed
		}

		protected abstract void UndoCommand();
		protected abstract void ExecuteCommand(bool redo);

		StateMachine<State> state = new StateMachine<State>();

		public virtual void Execute()
		{
			switch (state.state)
			{
				case State.Default:
					ExecuteCommand(false);
					state.State = State.Complete;
					break;
				case State.Reversed:
					ExecuteCommand(true);
					state.State = State.Complete;
					break;
				case State.Complete:
					Debug.Log("ERROR. Execute called twice for comand.");
					break;
				default:
					break;
			}
		}

		public void Undo()
		{
			if (State.Complete == state.state) {
				UndoCommand();
				state.State = State.Reversed;
			}
		}
	}
}
