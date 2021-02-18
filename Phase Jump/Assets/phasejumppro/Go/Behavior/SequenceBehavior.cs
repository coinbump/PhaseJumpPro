using UnityEngine;
using System.Collections;

/*
 * RATING: 4 stars. Has Unit Tests, could use visual tests
 * CODE REVIEW: 2.17.21
 */
namespace PJ
{
	/// <summary>
	/// Evaluates each child node in order, if all paths succeed, returns success
	/// </summary>
	public class SequenceBehavior : Behavior
	{
		protected int childIndex;

		protected override void EvtChildFinished(Behavior child, bool wasRunningNode)
		{
			base.EvtChildFinished(child, wasRunningNode);

			if (!wasRunningNode) { return; }

			switch (child.GetState())
			{
				case Behavior.State.Fail:
					state.State = State.Fail;
					return;
				case Behavior.State.Success:
					if (childIndex >= children.Count - 1)
					{
						state.State = State.Success;
						return;
					}
					else
					{
						childIndex++;
						Run();
					}
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

			for (; childIndex < children.Count; childIndex++)
			{
				var child = children[childIndex];
				var result = child.Run();

				switch (result)
				{
					case State.Fail:
						return result;
					case State.Running:
						// If child is running, sequence is still running
						return result;
				}
			}

			return State.Success;
		}
	}
}
