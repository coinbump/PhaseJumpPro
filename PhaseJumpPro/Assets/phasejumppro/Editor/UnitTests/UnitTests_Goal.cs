using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_Goal {
		[Test]
		public void TestIntGoal() {
			var repeatGoal = new IntGoal(2);

			Assert.AreEqual(0, repeatGoal.Progress);

			repeatGoal.Increment();
			Assert.IsFalse(repeatGoal.IsComplete);
			Assert.AreEqual(1, repeatGoal.ValueState);
			Assert.AreEqual(0.5f, repeatGoal.Progress);

			repeatGoal.Increment();
			Assert.IsTrue(repeatGoal.IsComplete);
			Assert.AreEqual(1.0f, repeatGoal.Progress);
		}
	}
}
