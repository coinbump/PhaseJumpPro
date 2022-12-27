using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_CycleAnimator
    {
        [Test]
        public void Test_CycleOnceForward()
        {
            var testValue = -1.0f;

            var sut = new CycleAnimator<float>(
                new FloatInterpolator(0, 360),
                new(1.0f, AnimationCycleType.Once),
                new SetBinding<float>((float value) => testValue = value)
            );

            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(0.5f, sut.Progress);
            Assert.AreEqual(180.0f, testValue);
            Assert.IsFalse(sut.IsFinished);

            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(1.0f, sut.Progress);
            Assert.AreEqual(360.0f, testValue);
            Assert.IsTrue(sut.IsFinished);
        }

        [Test]
        public void Test_MatchInReverse()
        {
            var testValue = -1.0f;

            var sut = new CycleAnimator<float>(
                new FloatInterpolator(0, 360),
                new(1.0f, AnimationCycleType.PingPong),
                new SetBinding<float>((float value) => testValue = value)
            );
            sut.reverseType = CycleAnimator<float>.ReverseType.Match;
            sut.interpolator.transform = _InterpolateType.easeInSquared;

            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(1.0f, sut.Progress);

            sut.OnUpdate(new TimeSlice(0.2f));
            Assert.AreEqual(0.8f, sut.Progress);
            Assert.AreEqual(360.0f * (1.0f - (0.2f * 0.2f)), testValue, float.Epsilon);
        }

        [Test]
        public void Test_RewindInReverse()
        {
            var testValue = -1.0f;

            var sut = new CycleAnimator<float>(
                new FloatInterpolator(0, 360),
                new(1.0f, AnimationCycleType.PingPong),
                new SetBinding<float>((float value) => testValue = value)
            );
            sut.reverseType = CycleAnimator<float>.ReverseType.Rewind;
            sut.interpolator.transform = _InterpolateType.easeInSquared;

            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(1.0f, sut.Progress);

            sut.OnUpdate(new TimeSlice(0.2f));
            Assert.AreEqual(0.8f, sut.Progress);
            Assert.AreEqual(360.0f * 0.8f * 0.8f, testValue, float.Epsilon);
        }
    }
}
