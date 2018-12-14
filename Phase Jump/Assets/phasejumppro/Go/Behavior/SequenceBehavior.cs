using UnityEngine;
using System.Collections;

/*
 * RATING: 4 stars. Has Unit Tests, could use visual tests
 * CODE REVIEW: 4.10.18
 */
namespace PJ
{
	/// <summary>
	/// Evaluates each child node in order, if all paths succeed, returns success
	/// </summary>
	public class SequenceBehavior : Behavior
	{
		protected int childIndex;

		protected override void EvtChildFinished(Behavior child)
		{
			base.EvtChildFinished(child);

			if (!IsRunning()) { return; }

			switch (child.GetState())
			{
				case Behavior.State.Fail:
					state.State = State.Fail;
					return;
				case Behavior.State.Success:
					if (childIndex == children.Count - 1)
					{
						state.State = State.Success;
						return;
					}
					else
					{
						childIndex++;
						_Run();
					}
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
			for (; childIndex < children.Count; childIndex++)
			{
				var child = children[childIndex];
				var result = child.Run();

				switch (result)
				{
					case State.Fail:
						state.State = result;
						return;
					case State.Success:
						if (childIndex == children.Count - 1) {
							state.State = State.Success;
							return;
						}
						break;
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
