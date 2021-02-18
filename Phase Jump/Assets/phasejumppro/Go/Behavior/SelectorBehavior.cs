using UnityEngine;
using System.Collections;

/*
 * RATING: 4 stars. Has Unit Tests, could use visual tests
 * CODE REVIEW: 2.17.21
 */
namespace PJ {

	/// <summary>
	/// Evaluates each child node in order, if one succeeds, returns success
	/// </summary>
	public class SelectorBehavior : Behavior
	{
		protected int childIndex;

		protected override void EvtChildFinished(Behavior child, bool wasRunningNode) {
			base.EvtChildFinished(child, wasRunningNode);

			if (!wasRunningNode) { return; }

			switch (child.GetState())
			{
				case Behavior.State.Fail:
					childIndex++;
					Run();
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

		protected override State Evaluate()
		{
			// As long as the children are being evaluated, we are running
			// Add to run stack before children
			state.State = State.Running;

			for (; childIndex < children.Count; childIndex++) {
				var child = children[childIndex];
				var result = child.Run();

				switch (result) {
					case State.Fail:
						break;
                    default:
						return result;
				}
			}

			return State.Fail;
		}
	}
}
