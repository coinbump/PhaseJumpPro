using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars. Simple design pattern with Unit Tests
 * CODE REVIEW: 4.10.18
 */
namespace PJ
{
	/// <summary>
	/// Specifies the weight of a random event
	/// </summary>
	public class Weight
	{
		private float _weight = 1.0f; // Store, if weight can be temporarily altered
		private float _adjustedWeight = 1.0f;

		// OPTIONAL:
		public string key;  // Key-value
		public AdjustWeight adjust;
		public object theObject;

		public Weight(float value, string key = "") {
			Value = value;
			this.key = key;
		}

		public Weight(float value, object theObject)
		{
			Value = value;
			this.theObject = theObject;
		}

		public float Value {
			get {
				return _weight;
			}
			set {
				_weight = value;
				_adjustedWeight = value;
			}
		}

		public float AdjustedValue
		{
			get
			{
				return _adjustedWeight;
			}
			set
			{
				_adjustedWeight = value;
			}
		}
	}

	/// <summary>
	/// Temporarily adjust the weight, based on the default weight.
	/// </summary>
	public abstract class AdjustWeight {
		public abstract void Adjust(Weight weight);	// Set the AdjustedValue based on DefaultValue
	}

	/// <summary>
	/// Each choice has a weight that affects its random probability
	/// Useful for cases where we want randomness, but prefer certain outcomes
	/// </summary>
	public class WeightedRandom : List<Weight>
	{
		public Weight ChooseRandom()
		{
			float factor = Random.Range(0, 1.0f);
			var result = ChooseFactor(factor);
			return result;
		}

		public Weight ChooseFactor(float factor)
		{
			float totalWeight = 0;
			foreach (Weight wr in this)
			{
				wr.AdjustedValue = wr.Value;
				if (wr.adjust != null)
				{
					wr.adjust.Adjust(wr);
				}
				totalWeight += wr.AdjustedValue;
			}

			// No viable choices
			if (totalWeight.Equals(0))
			{
				return null;
			}

			float random = totalWeight * factor;

			float curWeight = 0;
			foreach (Weight wr in this)
			{
				float weight = wr.AdjustedValue;
				if (weight <= 0)
				{
					continue;   // Ignore this, invalid
				}

				curWeight += weight;
				if (random <= curWeight)
				{
					return wr;
				}
			}

			return null;
		}
	}

}
