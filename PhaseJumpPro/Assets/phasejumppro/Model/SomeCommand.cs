using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4.13/22
 */
namespace PJ
{
	/// <summary>
	/// Commands are reversible. Typically a command will store a memento that allows it to reverse
	/// the state
	/// </summary>
	public abstract class SomeCommand
	{
		public enum StateType {
			Default,
			Complete,
			Reversed
		}

		protected abstract void OnUndo();
		protected abstract void OnExecute(bool redo);

		StateMachine<StateType> stateMachine = new StateMachine<StateType>();

		public StateType State => stateMachine.State;

		public virtual void Execute()
		{
			switch (State)
			{
				case StateType.Default:
					OnExecute(false);
					stateMachine.State = StateType.Complete;
					break;
				case StateType.Reversed:
					OnExecute(true);
					stateMachine.State = StateType.Complete;
					break;
				case StateType.Complete:
					Debug.Log("ERROR. Execute called twice for command.");
					break;
				default:
					break;
			}
		}

		public void Undo()
		{
			if (StateType.Complete == stateMachine.state) {
				OnUndo();
				stateMachine.State = StateType.Reversed;
			}
		}
	}
}
