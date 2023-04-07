using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_Animator
    {
        [Test]
        public void Test_Animate()
        {
            var testValue = -1.0f;

            var sut = new Animator<float>(
                new FloatInterpolator(0, 360),
                1.0f,
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
    }
}
