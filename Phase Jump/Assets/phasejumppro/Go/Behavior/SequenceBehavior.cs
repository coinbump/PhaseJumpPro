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
				case Behavior.State.Fail:
					state.State = State.Fail;
					return;
				case Behavior.State.Success:
					if (childIndex == children.Count - 1)
					{
						state.State = State.Success;
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
					case State.Fail:
						state.State = result;
						return;
					case State.Success:
						if (childIndex == children.Count - 1) {
							state.State = State.Success;
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
			Assert.AreEqual(Behavior.State.Fail, t1.GetState());
			Assert.AreEqual(Behavior.State.Fail, c1.GetState());

			c1.evaluate = true;
			c2.evaluate = false;

			t1.Run();
			Assert.AreEqual(Behavior.State.Fail, t1.GetState());
			Assert.AreEqual(Behavior.State.Success, c1.GetState());
			Assert.AreEqual(Behavior.State.Fail, c2.GetState());

			c1.evaluate = true;
			c2.evaluate = true;
			c1.evaluate = true;

			t1.Run();
			Assert.AreEqual(Behavior.State.Success, t1.GetState());
			Assert.AreEqual(Behavior.State.Success, c1.GetState());
			Assert.AreEqual(Behavior.State.Success, c2.GetState());
			Assert.AreEqual(Behavior.State.Success, c3.GetState());

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
