using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;

namespace PJ
{
    /// <summary>
    /// Unit Tests
    /// </summary>
    public class UnitTests_StateMachine
    {
        enum TestEnum
        {
            Invalid,
            Test1,
            Test2
        }

        private class TestStateMachine : StateMachine<TestEnum>
        {
            public int test1Count { get; protected set; }
            public int test2Count { get; protected set; }

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
            var sut = new TestStateMachine();
            sut.State = TestEnum.Test2;
            Assert.AreEqual(1, sut.test2Count);
            sut.IsLocked = true;

            sut.State = TestEnum.Test1;
            Assert.AreEqual(0, sut.test1Count);
            sut.IsLocked = false;
            sut.State = TestEnum.Test1;
            Assert.AreEqual(1, sut.test1Count);

            sut.State = TestEnum.Test2;
            Assert.AreEqual(TestEnum.Test1, sut.PrevState);
        }

        [Test]
        public void Test_StateMachine_Graph()
        {
            var sut = new TestStateMachine();
            var invalidNode = sut.AddState(TestEnum.Invalid);
            Assert.AreEqual(invalidNode, sut.NodeForState(TestEnum.Invalid));
            Assert.AreEqual(1, sut.nodes.Count);

            var inputs1 = new List<string>() { "test1" };
            sut.ConnectStates(TestEnum.Invalid, inputs1, TestEnum.Test1);

            var inputs2 = new List<string>() { "test2" };
            sut.ConnectStates(TestEnum.Test1, inputs2, TestEnum.Test2);

            Assert.AreEqual(3, sut.nodes.Count);
            sut.State = TestEnum.Invalid;
            sut.OnInput("test2");
            Assert.AreEqual(TestEnum.Invalid, sut.State);

            sut.OnInput("test1");
            Assert.AreEqual(TestEnum.Test1, sut.State);

            sut.OnInput("test2");
            Assert.AreEqual(TestEnum.Test2, sut.State);
        }
    }
}
