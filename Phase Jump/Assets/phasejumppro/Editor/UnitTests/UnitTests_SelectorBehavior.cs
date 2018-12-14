using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_SelectorBehavior
	{
		private class TestConditionBehavior : ConditionBehavior
		{
			public bool evaluate;
			public int runCount;

			public TestConditionBehavior()
			{
			}

			public TestConditionBehavior(bool evaluate)
			{
				this.evaluate = evaluate;
			}

			public override bool Evaluate()
			{
				return evaluate;
			}

			protected override void _Run()
			{
				base._Run();
				runCount++;
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
			var t1 = new SelectorBehavior();
			var c1 = new TestConditionBehavior(false);
			var c2 = new TestConditionBehavior(true);
			var c3 = new TestConditionBehavior(true);
			t1.AddChild(c1);
			t1.AddChild(c2);
			t1.AddChild(c3);

			t1.Run();
			Assert.AreEqual(1, c1.runCount);
			Assert.AreEqual(1, c2.runCount);
			Assert.IsFalse(c1.IsRunning());
			Assert.IsFalse(c2.IsRunning());
			Assert.AreEqual(null, t1.RunningChild);
			Assert.AreEqual(Behavior.State.Success, t1.GetState());
			Assert.AreEqual(Behavior.State.Fail, c1.GetState());
			Assert.AreEqual(Behavior.State.Success, c2.GetState());

			c1.evaluate = true;
			c2.evaluate = false;

			t1.Run();
			Assert.AreEqual(Behavior.State.Success, t1.GetState());
			Assert.AreEqual(Behavior.State.Success, c1.GetState());

			c1.evaluate = false;
			t1.Run();
			Assert.AreEqual(Behavior.State.Success, t1.GetState());
			Assert.AreEqual(Behavior.State.Fail, c1.GetState());
			Assert.AreEqual(Behavior.State.Fail, c2.GetState());
			Assert.AreEqual(Behavior.State.Success, c3.GetState());

			c3.evaluate = false;
			t1.Run();
			Assert.AreEqual(Behavior.State.Fail, t1.GetState());
			Assert.AreEqual(Behavior.State.Fail, c1.GetState());
			Assert.AreEqual(Behavior.State.Fail, c2.GetState());
			Assert.AreEqual(Behavior.State.Fail, c3.GetState());

			var c0 = new TestTimedBehavior();
			t1.AddChild(c0);
			t1.Run();
			Assert.AreEqual(t1.RunningChild, c0);
			Assert.IsTrue(t1.IsRunning());
			t1.EvtUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(null, t1.RunningChild);
			Assert.AreEqual(Behavior.State.Success, t1.GetState());
			Assert.AreEqual(Behavior.State.Success, c0.GetState());
		}
	}
}
