using System;

/*
 * RATING: 5 stars
 * Simple interface
 * CODE REVIEW: 4/21/22
 */
namespace PJ
{
	/// <summary>
	/// Temporarily adjust the weight, based on the default weight
	/// </summary>
	public abstract class SomeAdjustWeight
	{
		public abstract float AdjustedWeightFor(Weight weight);
	}
}
