using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_Behavior
	{

		private class TestConditionBehavior : ConditionBehavior
		{
			public bool evaluate;

			public override bool EvaluateCondition()
			{
				return evaluate;
			}
		}

		private class TestTimedBehavior : TimedBehavior
		{
			public TestTimedBehavior()
				: base(1.0f)
			{

			}
		}

		[Test]
		public void UnitTests()
		{
			var root = new RootBehavior(new WeakReference(this));
			var t1 = new Behavior();
			var c1 = new TestConditionBehavior();
			var c11 = new TestTimedBehavior();
			var c12 = new Behavior();

			root.AddChild(t1);
			t1.AddChild(c1);
			Assert.AreEqual(c1.owner, t1.owner);

			t1.Run();
			Assert.AreEqual(Behavior.State.Fail, c1.GetState());
			c1.evaluate = true;
			t1.Run();
			Assert.AreEqual(Behavior.State.Success, c1.GetState());

			c1.AddChild(c11);
			c1.AddChild(c12);

			Assert.AreEqual(root, t1.RootNode());
			Assert.AreEqual(root, c1.RootNode());
			Assert.AreEqual(root, c11.RootNode());
			Assert.AreEqual(root, c12.RootNode());

			t1.Run();
			Assert.AreEqual(t1.RunningNode, c11);
			Assert.IsTrue(c11.IsRunning());
			t1.Run();
			t1.EvtUpdate(new TimeSlice(1.0f));
			Assert.IsTrue(c11.IsFinished());
			Assert.IsNull(t1.RunningNode);
		}
	}
}
