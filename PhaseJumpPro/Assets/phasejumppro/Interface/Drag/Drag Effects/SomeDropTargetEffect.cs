using System;

/*
 * RATING: 5 stars
 * Simple abstract class
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    using StateType = DropTarget.StateType;

	/// <summary>
	/// When a drag target has a drag over it, add an effect
	/// </summary>
	public abstract class SomeDropTargetEffect : MonoBehaviour
	{
        protected StateType state = StateType.NotOver;

		public StateType State
		{
			get => state;
			set
			{
				if (state == value) { return; }
				state = value;
				OnStateChange();
			}
		}

        protected abstract void OnStateChange();
    }
}
