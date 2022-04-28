using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/27/22
 */
namespace PJ
{
	/// <summary>
    /// A goal that must reach a value target to be complete
    /// </summary>
	public class IntGoal : SomeGoal
	{
		protected int valueTarget;
		protected int valueState;

		public int ValueTarget
        {
			get => valueTarget;
			set
            {
				valueTarget = value;
            }
        }

		public int ValueState => valueState;

		public IntGoal(int valueTarget, int valueState = 0)
		{
			this.valueTarget = valueTarget;
			this.valueState = valueState;
		}

		public override float Progress => Mathf.Max(0, MathF.Min(1.0f, (float)valueState / (float)valueTarget));

		public void Increment()
		{
			if (valueState >= valueTarget) { return; }

			valueState++;
			if (valueState >= valueTarget)
			{
				IsComplete = true;
			}
		}
	}
}
