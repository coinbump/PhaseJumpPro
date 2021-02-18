using UnityEngine;
using System.Collections;

/*
 * RATING: 4 stars. Has Unit Tests, could use visual tests
 * CODE REVIEW: 2.17.21
 */
namespace PJ {

	/// <summary>
	/// Evaluates a condition and if true, continues to first child node
	/// Unit Tests are with Behavior
	/// </summary>
	public abstract class ConditionBehavior : Behavior
	{
		public abstract bool EvaluateCondition();

		protected override State Evaluate()
		{
			if (EvaluateCondition())
			{
				if (children.Count > 0)
				{
					var child = children[0];
					child.Run();
				}
				return State.Success;
			}
			else
			{
				return State.Fail;
			}
		}
	}
}
