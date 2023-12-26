using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_TimedSequence
    {
        [Test]
        public void Test_TimedSequence()
        {
            var sut = new TimedSequence();
            var timer1 = new Timer(1.0f, SomeRunner.RunType.RunOnce);
            var timer2 = new Timer(2.0f, SomeRunner.RunType.RunOnce);
            sut.Add(timer1);
            sut.Add(timer2);

            Assert.IsFalse(sut.IsFinished);

            sut.OnUpdate(new TimeSlice(0.5f));

            Assert.IsFalse(sut.IsFinished);
            Assert.AreEqual(0.5f, timer1.Progress);

            sut.OnUpdate(new TimeSlice(1.5f));

            Assert.IsFalse(sut.IsFinished);
            Assert.AreEqual(1.0f, timer1.Progress);
            Assert.IsTrue(timer1.IsFinished);
            Assert.AreEqual(0.5f, timer2.Progress);

            sut.OnUpdate(new TimeSlice(2.0f));
            Assert.IsTrue(sut.IsFinished);
            Assert.IsTrue(timer2.IsFinished);
            Assert.AreEqual(1.0f, timer2.Progress);
        }
    }
}
