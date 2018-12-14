using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_Goal {
		[Test]
		public void UnitTests() {
			var repeatGoal = new Goal();
			repeatGoal.repeatTarget = 2;
			repeatGoal.Increment();
			Assert.AreEqual(1, repeatGoal.repeatCount);
			repeatGoal.Increment();
			Assert.IsTrue(repeatGoal.IsComplete);

			var normalGoal = new Goal();
			normalGoal.ProgressNormal = 1.0f;
			Assert.IsTrue(normalGoal.IsComplete);

			var intGoal = new Goal();
			intGoal.TargetInt = 2;
			Assert.IsFalse(intGoal.IsComplete);
			intGoal.ProgressInt = 2;
			Assert.IsTrue(intGoal.IsComplete);
		}

		[Test]
		public void UnitTestsTree() {
			var repeatGoal = new Goal();
			repeatGoal.repeatTarget = 2;

			var normalGoal = new Goal();

			var intGoal = new Goal();
			intGoal.TargetInt = 2;

			var treeGoal = new Goal();
			treeGoal.AddChildGoal(repeatGoal);	
			treeGoal.AddChildGoal(normalGoal);	
			treeGoal.AddChildGoal(intGoal);		
			
			repeatGoal.Increment();
			Assert.AreEqual(1, repeatGoal.repeatCount);
			repeatGoal.Increment();
			Assert.IsTrue(repeatGoal.IsComplete);
			Assert.IsFalse(treeGoal.IsComplete);
			normalGoal.ProgressNormal = 1.0f;
			Assert.IsTrue(normalGoal.IsComplete);
			Assert.IsFalse(treeGoal.IsComplete);
			Assert.IsFalse(intGoal.IsComplete);
			intGoal.ProgressInt = 2;
			Assert.IsTrue(intGoal.IsComplete);
			Assert.IsTrue(treeGoal.IsComplete);
		}

	}

}

