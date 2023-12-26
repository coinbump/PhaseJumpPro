using System;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_Timer
    {
        private class TestTimer : Timer
        {
            public int finishedCount = 0;

            public TestTimer(float duration, SomeRunner.RunType runType) : base(duration, runType)
            {
            }

            protected override void OnFinish()
            {
                base.OnFinish();

                finishedCount++;
            }
        }

        [Test]
        public void TestTimer_RunOnce()
        {
            var timer = new Timer(1.0f, PJ.SomeRunner.RunType.RunOnce);
            timer.OnUpdate(new TimeSlice(.3f));
            Assert.AreEqual(.3f, timer.TimerState);
            Assert.AreEqual(.3f, timer.Progress);
            Assert.IsFalse(timer.IsFinished);

            timer.OnUpdate(new TimeSlice(.7f));
            Assert.AreEqual(1.0f, timer.TimerState);
            Assert.AreEqual(1.0f, timer.Progress);
            Assert.IsTrue(timer.IsFinished);

            timer.OnUpdate(new TimeSlice(.7f));
            Assert.AreEqual(1.0f, timer.TimerState);
            Assert.AreEqual(1.0f, timer.Progress);
            Assert.IsTrue(timer.IsFinished);

            timer.SetProgress(.5f);
            Assert.IsFalse(timer.IsFinished);
        }

        [Test]
        public void TestTimer_KeepRunning()
        {
            var timer = new TestTimer(1.0f, SomeRunner.RunType.KeepRunning);
            timer.OnUpdate(new TimeSlice(.3f));
            Assert.AreEqual(0.3f, timer.TimerState);
            Assert.AreEqual(0.3f, timer.Progress);

            timer.OnUpdate(new TimeSlice(.7f));
            Assert.AreEqual(0, timer.TimerState);
            Assert.AreEqual(0, timer.Progress);
            Assert.AreEqual(1, timer.finishedCount);
            Assert.IsFalse(timer.IsFinished);

            timer.OnUpdate(new TimeSlice(.7f));
            Assert.AreEqual(0.7f, timer.TimerState);
            Assert.AreEqual(0.7f, timer.Progress);
            Assert.IsFalse(timer.IsFinished);

            timer.OnUpdate(new TimeSlice(.3f));
            Assert.AreEqual(0, timer.TimerState);
            Assert.AreEqual(0, timer.Progress);
            Assert.AreEqual(2, timer.finishedCount);
            Assert.IsFalse(timer.IsFinished);
        }
    }
}
