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
	class UnitTests_WeightedRandom
	{
		[Test]
		public void UnitTests()
		{
			var test = new WeightedRandom();
			var w1 = new Weight(1.0f);
			var w2 = new Weight(0.5f);
			var w3 = new Weight(0.25f);
			var totalWeight = w1.Value + w2.Value + w3.Value;

			test.Add(w1);
			test.Add(w2);
			test.Add(w3);

			Assert.AreEqual(w1, test.ChooseFactor(.5f / totalWeight));
			Assert.AreEqual(w1, test.ChooseFactor(.9f / totalWeight));
			Assert.AreEqual(w2, test.ChooseFactor(1.1f / totalWeight));
			Assert.AreEqual(w3, test.ChooseFactor(1.6f / totalWeight));
		}
	}

}
