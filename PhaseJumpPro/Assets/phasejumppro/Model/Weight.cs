using System;

/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 4/21/22
 */
namespace PJ
{
	/// <summary>
	/// Specifies the weight of a random event
	/// </summary>
	public class Weight
	{
		/// <summary>
		/// Base weight (never changes)
		/// </summary>
		protected float weight = 1.0f;

		public string id;

		/// <summary>
		/// Optional object that dynamically adjusts the weight during evaluation
		/// </summary>
		public SomeValueTransform<Weight> adjust;

		/// <summary>
		/// Optional object that corresponds with the weight
		/// </summary>
		public object target;

		public Weight(float weight, string id = "")
		{
			this.weight = weight;
			this.id = id;
		}

		public Weight(float weight, object target)
		{
			this.weight = weight;
			this.target = target;
		}

		public float Value
		{
			get => weight;
			set
			{
				weight = value;
			}
		}
	}
}
