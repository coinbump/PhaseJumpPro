using UnityEngine;
using System.Collections;

/*
 * RATING: 4 stars. Has Unit Tests, could use visual tests
 * CODE REVIEW: 4.10.18
 */
namespace PJ {

	/// <summary>
	/// Evaluates each child node in order, if one succeeds, returns success
	/// </summary>
	public class SelectorBehavior : Behavior
	{
		protected int childIndex;

		protected override void EvtChildFinished(Behavior child) {
			base.EvtChildFinished(child);

			if (!IsRunning()) { return; }

			switch (child.GetState())
			{
				case Behavior.State.Fail:
					childIndex++;
					_Run();
					return;
				case Behavior.State.Success:
					state.State = State.Success;
					break;
			}
		}

		protected override void EvtFinished()
		{
			base.EvtFinished();

			childIndex = 0;
		}

		protected override void _Run()
		{
			for (; childIndex < children.Count; childIndex++) {
				var child = children[childIndex];
				var result = child.Run();

				switch (result) {
					case State.Fail:
						state.State = result;
						break;
					case State.Success:
						state.State = result;
						return;
				}

				if (child.IsRunning())
				{
					state.State = State.RunningChild;
					return;
				}
			}
		}
	}
}
