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

			public override bool Evaluate()
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
			var t1 = new Behavior(new WeakReference(this));
			var c1 = new TestConditionBehavior();
			var c11 = new TestTimedBehavior();
			var c12 = new Behavior();

			t1.AddChild(c1);
			Assert.AreEqual(c1.owner, t1.owner);

			t1.Run();
			Assert.AreEqual(Behavior.State.Fail, c1.GetState());
			c1.evaluate = true;
			t1.Run();
			Assert.AreEqual(Behavior.State.Success, c1.GetState());

			c1.AddChild(c11);
			c1.AddChild(c12);

			Assert.AreEqual(t1, t1.RootNode());
			Assert.AreEqual(t1, c1.RootNode());
			Assert.AreEqual(t1, c11.RootNode());
			Assert.AreEqual(t1, c12.RootNode());

			t1.Run();
			Assert.AreEqual(t1.RunningChild, c11);
			Assert.IsTrue(c11.IsRunning());
			t1.Run();
			t1.EvtUpdate(new TimeSlice(1.0f));
			Assert.IsTrue(c11.IsFinished());
		}
	}
}
