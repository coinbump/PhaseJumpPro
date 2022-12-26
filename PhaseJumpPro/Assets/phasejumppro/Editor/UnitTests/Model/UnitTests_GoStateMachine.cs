using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;

namespace PJ
{
    /// <summary>
    /// Unit Tests
    /// </summary>
    public class UnitTests_GoStateMachine
    {
        enum TestEnum
        {
            Invalid,
            Test1,
            Test2
        }

        private class TestStateMachine : GoStateMachine<TestEnum>
        {
            public int test1Count { get; protected set; }
            public int test2Count { get; protected set; }
            public int finishedCount { get; protected set; }

            protected override void OnStateFinish()
            {
                base.OnStateFinish();

                finishedCount++;
            }

            protected override void OnStateChange(TestEnum newState)
            {
                base.OnStateChange(newState);

                switch (newState)
                {
                    case TestEnum.Test1:
                        test1Count++;
                        break;
                    case TestEnum.Test2:
                        test2Count++;
                        break;
                }
            }
        }

        [Test]
        public void Test_StateMachine()
        {
            var test = new TestStateMachine();
            test.State = TestEnum.Test2;
            Assert.AreEqual(1, test.test2Count);
            test.IsLocked = true;

            test.State = TestEnum.Test1;
            Assert.AreEqual(0, test.test1Count);
            test.IsLocked = false;
            test.State = TestEnum.Test1;
            Assert.AreEqual(1, test.test1Count);

            test.SetStateDuration(1.0f);
            test.OnUpdate(new TimeSlice(1.5f));
            Assert.AreEqual(test.finishedCount, 1);
            test.ResetStateTimer();
            test.OnUpdate(new TimeSlice(.5f));
            Assert.AreEqual(.5f, test.StateProgress, .001f);
            test.OnUpdate(new TimeSlice(.6f));
            Assert.AreEqual(test.finishedCount, 2);

            test.State = TestEnum.Test2;
            Assert.AreEqual(TestEnum.Test1, test.PrevState);
        }

        [Test]
        public void Test_StateMachine_Modifiers()
        {
            var sut = new TestStateMachine();
            var inputs1 = new List<string>() { "test1" };
            sut.ConnectStates(TestEnum.Invalid, inputs1, TestEnum.Test1);
            var inputs2 = new List<string>() { "test2" };
            sut.ConnectStates(TestEnum.Test1, inputs2, TestEnum.Test2);
            sut.ConnectStates(TestEnum.Test2, inputs2, TestEnum.Invalid);

            sut.modifiers.Add(new TestStateMachine.TimedThenInputModifier(1.0f, "test1", sut, SomeRunner.RunType.RunOnce));

            sut.State = TestEnum.Invalid;

            // Test Run Once
            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(TestEnum.Test1, sut.State);

            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(TestEnum.Test1, sut.State);

            Assert.AreEqual(0, sut.modifiers.Count);

            // Test Keep Running
            sut.modifiers.Add(new TestStateMachine.TimedThenInputModifier(1.0f, "test2", sut, SomeRunner.RunType.KeepRunning));
            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(TestEnum.Test2, sut.State);

            Assert.AreEqual(1, sut.modifiers.Count);

            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(TestEnum.Invalid, sut.State);
        }
    }
}
