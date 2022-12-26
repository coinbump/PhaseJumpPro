using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_GoCore
    {
        enum StateType
        {
            Invalid, Left, Right
        }

        private class TestObject : GoCore<StateType>
        {
            public int stateChangeCount = 0;
            public int stateFinishCount = 0;

            protected override void OnStateChange(GoStateMachine<StateType> inStateMachine)
            {
                stateChangeCount++;
            }

            protected override void OnStateFinish(GoStateMachine<StateType> inStateMachine)
            {
                stateFinishCount++;
            }
        }

        [Test]
        public void Test_StateChange()
        {
            var sut = new TestObject();

            Assert.AreEqual(0, sut.stateChangeCount);
            sut.sm.State = StateType.Right;
            Assert.AreEqual(1, sut.stateChangeCount);
            sut.sm.State = StateType.Right;
            Assert.AreEqual(1, sut.stateChangeCount);
            sut.sm.State = StateType.Left;
            Assert.AreEqual(2, sut.stateChangeCount);
        }

        [Test]
        public void Test_StateFinish()
        {
            var sut = new TestObject();

            sut.sm.State = StateType.Right;
            Assert.AreEqual(0, sut.stateFinishCount);
            sut.sm.SetStateDuration(1.0f);

            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(1, sut.stateFinishCount);

            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(1, sut.stateFinishCount);
        }
    }
}
