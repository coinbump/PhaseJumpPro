using UnityEngine;
using System.Collections;

namespace PJ {

	/// <summary>
	/// Evaluates a condition and if true, continues to first child node
	/// Unit Tests are with Behavior
	/// </summary>
	public abstract class ConditionBehavior : Behavior
	{
		public abstract bool Evaluate();

		protected override void _Run()
		{
			if (Evaluate())
			{
				state.State = State.Success;
				if (children.Count > 0)
				{
					var child = children[0];
					child.Run();
				}
			}
			else
			{
				state.State = State.Fail;
			}
		}
	}
}
