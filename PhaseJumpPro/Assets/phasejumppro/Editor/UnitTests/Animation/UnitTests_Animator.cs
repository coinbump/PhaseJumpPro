using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_Animator
	{
		[Test]
		public void TestAnimator_CycleOnceForward()
		{
			var animator = new Animator();
			animator.CycleType = AnimationCycleType.Once;
			animator.CycleTime = 1.0f;

			Assert.AreEqual(AnimationCycleState.Forward, animator.CycleState);
			Assert.AreEqual(0, animator.Progress);

			animator.OnUpdate(new TimeSlice(0.3f));

			Assert.AreEqual(AnimationCycleState.Forward, animator.CycleState);
			Assert.AreEqual(.3f, animator.Progress);

			Assert.IsFalse(animator.IsFinished);
			animator.OnUpdate(new TimeSlice(0.7f));
			Assert.True(animator.IsFinished);

			Assert.AreEqual(AnimationCycleState.Forward, animator.CycleState);
			Assert.AreEqual(1.0f, animator.Progress);
		}

		[Test]
		public void TestAnimator_CycleOnceReverse()
		{
			var animator = new Animator();
			animator.CycleType = AnimationCycleType.Once;
			animator.CycleTime = 1.0f;
			animator.CycleState = AnimationCycleState.Reverse;
			animator.SetProgress(1.0f);

			Assert.AreEqual(AnimationCycleState.Reverse, animator.CycleState);
			Assert.AreEqual(1.0f, animator.Progress);

			animator.OnUpdate(new TimeSlice(.3f));

			Assert.AreEqual(AnimationCycleState.Reverse, animator.CycleState);
			Assert.AreEqual(.7f, animator.Progress);

			Assert.IsFalse(animator.IsFinished);
			animator.OnUpdate(new TimeSlice(0.7f));
			Assert.True(animator.IsFinished);

			Assert.AreEqual(AnimationCycleState.Reverse, animator.CycleState);
			Assert.AreEqual(0, animator.Progress);
		}

		[Test]
		public void TestAnimator_CycleLoopForward()
		{
			var animator = new Animator();
			animator.CycleType = AnimationCycleType.Loop;
			animator.CycleTime = 1.0f;

			Assert.AreEqual(AnimationCycleState.Forward, animator.CycleState);
			Assert.AreEqual(0, animator.Progress);

			Assert.IsFalse(animator.IsFinished);
			animator.OnUpdate(new TimeSlice(1.0f));
			Assert.False(animator.IsFinished);

			Assert.AreEqual(AnimationCycleState.Forward, animator.CycleState);
			Assert.AreEqual(0, animator.Progress);

			animator.OnUpdate(new TimeSlice(.3f));
			Assert.AreEqual(AnimationCycleState.Forward, animator.CycleState);
			Assert.AreEqual(.3f, animator.Progress);
		}

		[Test]
		public void TestAnimator_CycleLoopReverse()
		{
			var animator = new Animator();
			animator.CycleType = AnimationCycleType.Loop;
			animator.CycleTime = 1.0f;
			animator.CycleState = AnimationCycleState.Reverse;
			animator.SetProgress(1.0f);

			Assert.AreEqual(AnimationCycleState.Reverse, animator.CycleState);
			Assert.AreEqual(1.0f, animator.Progress);

			Assert.IsFalse(animator.IsFinished);
			animator.OnUpdate(new TimeSlice(1.0f));
			Assert.False(animator.IsFinished);

			Assert.AreEqual(AnimationCycleState.Reverse, animator.CycleState);
			Assert.AreEqual(1.0f, animator.Progress);

			animator.OnUpdate(new TimeSlice(.3f));
			Assert.AreEqual(AnimationCycleState.Reverse, animator.CycleState);
			Assert.AreEqual(.7f, animator.Progress);
		}

		[Test]
		public void TestAnimator_CyclePingPongForward()
		{
			var animator = new Animator();
			animator.CycleType = AnimationCycleType.PingPong;
			animator.CycleTime = 1.0f;

			Assert.AreEqual(AnimationCycleState.Forward, animator.CycleState);
			Assert.AreEqual(0, animator.Progress);

			Assert.IsFalse(animator.IsFinished);
			animator.OnUpdate(new TimeSlice(1.0f));
			Assert.False(animator.IsFinished);

			Assert.AreEqual(AnimationCycleState.Reverse, animator.CycleState);
			Assert.AreEqual(1.0f, animator.Progress);

			animator.OnUpdate(new TimeSlice(.3f));
			Assert.AreEqual(AnimationCycleState.Reverse, animator.CycleState);
			Assert.AreEqual(.7f, animator.Progress);
		}

		[Test]
		public void TestAnimator_CyclePingPongReverse()
		{
			var animator = new Animator();
			animator.CycleType = AnimationCycleType.PingPong;
			animator.CycleTime = 1.0f;
			animator.CycleState = AnimationCycleState.Reverse;
			animator.SetProgress(1.0f);

			Assert.AreEqual(AnimationCycleState.Reverse, animator.CycleState);
			Assert.AreEqual(1.0f, animator.Progress);

			Assert.IsFalse(animator.IsFinished);
			animator.OnUpdate(new TimeSlice(1.0f));
			Assert.False(animator.IsFinished);

			Assert.AreEqual(AnimationCycleState.Forward, animator.CycleState);
			Assert.AreEqual(0, animator.Progress);

			animator.OnUpdate(new TimeSlice(.3f));
			Assert.AreEqual(AnimationCycleState.Forward, animator.CycleState);
			Assert.AreEqual(.3f, animator.Progress);
		}
	}
}
