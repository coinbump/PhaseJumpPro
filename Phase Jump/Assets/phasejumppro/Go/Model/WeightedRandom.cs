using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

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
		private float _weight = 1.0f;
		private float _defaultWeight = 1.0f; // Store, if weight can be temporarily altered

		// OPTIONAL:
		public string key;  // Key-value
		public AdjustWeight adjust;

		public Weight(float value) {
			Value = value;
		}

		public float Value {
			get {
				return _weight;
			}
			set {
				_defaultWeight = value;
				_weight = value;
			}
		}

		public float AdjustedValue
		{
			get
			{
				return _weight;
			}
			set
			{
				_weight = value;
			}
		}

		public float DefaultValue
		{
			get
			{
				return _defaultWeight;
			}
			set
			{
				_defaultWeight = value;
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
	class WeightedRandom : List<Weight>
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
				if (wr.adjust != null) {
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

		[Test]
		public void UnitTests() {
			var test = new WeightedRandom();
			var w1 = new Weight(1.0f);
			var w2 = new Weight(0.5f);
			var w3 = new Weight(0.25f);
			var totalWeight = w1.Value + w2.Value + w3.Value;

			test.Add(w1);
			test.Add(w2);
			test.Add(w3);

			Assert.AreEqual(w1, test.ChooseFactor(.5f/totalWeight));
			Assert.AreEqual(w1, test.ChooseFactor(.9f/totalWeight));
			Assert.AreEqual(w2, test.ChooseFactor(1.1f/totalWeight));
			Assert.AreEqual(w3, test.ChooseFactor(1.6f/totalWeight));
		}
	}

}
