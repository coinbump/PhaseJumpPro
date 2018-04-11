using UnityEngine;
using System.Collections;
using NUnit.Framework;

/*
 * RATING: 4 stars. Has Unit Tests, could use visual tests
 * CODE REVIEW: 4.10.18
 */
namespace PJ
{
	/// <summary>
	/// Evaluates each child node in order, if all paths succeed, returns success
	/// </summary>
	public class SequenceBehavior : Behavior
	{
		protected int childIndex;

		protected override void EvtChildFinished(Behavior child)
		{
			base.EvtChildFinished(child);

			if (!IsRunning()) { return; }

			switch (child.GetState())
			{
				case Behavior.State.FinishedFail:
					state.State = State.FinishedFail;
					return;
				case Behavior.State.FinishedSuccess:
					if (childIndex == children.Count - 1)
					{
						state.State = State.FinishedSuccess;
						return;
					}
					else
					{
						childIndex++;
						_Run();
					}
					break;
			}
		}

		protected override void EvtFinished()
		{
			base.EvtFinished();

			childIndex = 0;
		}

		protected override void _Run()
		{
			for (; childIndex < children.Count; childIndex++)
			{
				var child = children[childIndex];
				var result = child.Run();

				switch (result)
				{
					case State.FinishedFail:
						state.State = result;
						return;
					case State.FinishedSuccess:
						if (childIndex == children.Count - 1) {
							state.State = State.FinishedSuccess;
							return;
						}
						break;
				}

				if (child.IsRunning())
				{
					state.State = State.RunningChild;
					return;
				}
			}
		}
	}

	public class UnitTests_SequenceBehavior
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

		private class TestTimerBehavior : TimerBehavior
		{
			public TestTimerBehavior()
				: base(1.0f)
			{

			}
		}

		[Test]
		public void UnitTests()
		{
			var t1 = new SequenceBehavior();
			var c1 = new TestConditionBehavior(false);
			var c2 = new TestConditionBehavior(true);
			var c3 = new TestConditionBehavior(true);
			t1.AddChild(c1);
			t1.AddChild(c2);
			t1.AddChild(c3);

			t1.Run();
			Assert.AreEqual(1, c1.runCount);
			Assert.AreEqual(0, c2.runCount);
			Assert.IsFalse(c1.IsRunning());
			Assert.IsFalse(c2.IsRunning());
			Assert.AreEqual(null, t1.RunningChild);
			Assert.AreEqual(Behavior.State.FinishedFail, t1.GetState());
			Assert.AreEqual(Behavior.State.FinishedFail, c1.GetState());

			c1.evaluate = true;
			c2.evaluate = false;

			t1.Run();
			Assert.AreEqual(Behavior.State.FinishedFail, t1.GetState());
			Assert.AreEqual(Behavior.State.FinishedSuccess, c1.GetState());
			Assert.AreEqual(Behavior.State.FinishedFail, c2.GetState());

			c1.evaluate = true;
			c2.evaluate = true;
			c1.evaluate = true;

			t1.Run();
			Assert.AreEqual(Behavior.State.FinishedSuccess, t1.GetState());
			Assert.AreEqual(Behavior.State.FinishedSuccess, c1.GetState());
			Assert.AreEqual(Behavior.State.FinishedSuccess, c2.GetState());
			Assert.AreEqual(Behavior.State.FinishedSuccess, c3.GetState());

			var c0 = new TestTimerBehavior();
			t1.AddChild(c0);
			t1.Run();
			Assert.AreEqual(t1.RunningChild, c0);
			Assert.IsTrue(t1.IsRunning());
			t1.EvtUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(null, t1.RunningChild);
			Assert.AreEqual(Behavior.State.FinishedSuccess, t1.GetState());
			Assert.AreEqual(Behavior.State.FinishedSuccess, c0.GetState());
		}
	}
}
