using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_AnimationCycleTimer
	{
		[Test]
		public void TestAnimator_CycleOnceForward()
		{
			var sut = new AnimationCycleTimer(1.0f, AnimationCycleType.Once);

			Assert.AreEqual(AnimationCycleState.Forward, sut.CycleState);
			Assert.AreEqual(0, sut.Progress);

			sut.OnUpdate(new TimeSlice(0.3f));

			Assert.AreEqual(AnimationCycleState.Forward, sut.CycleState);
			Assert.AreEqual(.3f, sut.Progress);

			Assert.IsFalse(sut.IsFinished);
			sut.OnUpdate(new TimeSlice(0.7f));
			Assert.True(sut.IsFinished);

			Assert.AreEqual(AnimationCycleState.Forward, sut.CycleState);
			Assert.AreEqual(1.0f, sut.Progress);
		}

		[Test]
		public void TestAnimator_CycleOnceReverse()
		{
			var sut = new AnimationCycleTimer(1.0f, AnimationCycleType.Once);
			sut.CycleState = AnimationCycleState.Reverse;
			sut.SetProgress(1.0f);

			Assert.AreEqual(AnimationCycleState.Reverse, sut.CycleState);
			Assert.AreEqual(1.0f, sut.Progress);

			sut.OnUpdate(new TimeSlice(.3f));

			Assert.AreEqual(AnimationCycleState.Reverse, sut.CycleState);
			Assert.AreEqual(.7f, sut.Progress);

			Assert.IsFalse(sut.IsFinished);
			sut.OnUpdate(new TimeSlice(0.7f));
			Assert.True(sut.IsFinished);

			Assert.AreEqual(AnimationCycleState.Reverse, sut.CycleState);
			Assert.AreEqual(0, sut.Progress);
		}

		[Test]
		public void TestAnimator_CycleLoopForward()
		{
			var sut = new AnimationCycleTimer(1.0f, AnimationCycleType.Loop);

			Assert.AreEqual(AnimationCycleState.Forward, sut.CycleState);
			Assert.AreEqual(0, sut.Progress);

			Assert.IsFalse(sut.IsFinished);
			sut.OnUpdate(new TimeSlice(1.0f));
			Assert.False(sut.IsFinished);

			Assert.AreEqual(AnimationCycleState.Forward, sut.CycleState);
			Assert.AreEqual(0, sut.Progress);

			sut.OnUpdate(new TimeSlice(.3f));
			Assert.AreEqual(AnimationCycleState.Forward, sut.CycleState);
			Assert.AreEqual(.3f, sut.Progress);
		}

		[Test]
		public void TestAnimator_CycleLoopReverse()
		{
			var sut = new AnimationCycleTimer(1.0f, AnimationCycleType.Loop);
			sut.CycleState = AnimationCycleState.Reverse;
			sut.SetProgress(1.0f);

			Assert.AreEqual(AnimationCycleState.Reverse, sut.CycleState);
			Assert.AreEqual(1.0f, sut.Progress);

			Assert.IsFalse(sut.IsFinished);
			sut.OnUpdate(new TimeSlice(1.0f));
			Assert.False(sut.IsFinished);

			Assert.AreEqual(AnimationCycleState.Reverse, sut.CycleState);
			Assert.AreEqual(1.0f, sut.Progress);

			sut.OnUpdate(new TimeSlice(.3f));
			Assert.AreEqual(AnimationCycleState.Reverse, sut.CycleState);
			Assert.AreEqual(.7f, sut.Progress);
		}

		[Test]
		public void TestAnimator_CyclePingPongForward()
		{
			var sut = new AnimationCycleTimer(1.0f, AnimationCycleType.PingPong);

			Assert.AreEqual(AnimationCycleState.Forward, sut.CycleState);
			Assert.AreEqual(0, sut.Progress);

			Assert.IsFalse(sut.IsFinished);
			sut.OnUpdate(new TimeSlice(1.0f));
			Assert.False(sut.IsFinished);

			Assert.AreEqual(AnimationCycleState.Reverse, sut.CycleState);
			Assert.AreEqual(1.0f, sut.Progress);

			sut.OnUpdate(new TimeSlice(.3f));
			Assert.AreEqual(AnimationCycleState.Reverse, sut.CycleState);
			Assert.AreEqual(.7f, sut.Progress);
		}

		[Test]
		public void TestAnimator_CyclePingPongReverse()
		{
			var sut = new AnimationCycleTimer(1.0f, AnimationCycleType.PingPong);
			sut.CycleState = AnimationCycleState.Reverse;
			sut.SetProgress(1.0f);

			Assert.AreEqual(AnimationCycleState.Reverse, sut.CycleState);
			Assert.AreEqual(1.0f, sut.Progress);

			Assert.IsFalse(sut.IsFinished);
			sut.OnUpdate(new TimeSlice(1.0f));
			Assert.False(sut.IsFinished);

			Assert.AreEqual(AnimationCycleState.Forward, sut.CycleState);
			Assert.AreEqual(0, sut.Progress);

			sut.OnUpdate(new TimeSlice(.3f));
			Assert.AreEqual(AnimationCycleState.Forward, sut.CycleState);
			Assert.AreEqual(.3f, sut.Progress);
		}
	}
}
