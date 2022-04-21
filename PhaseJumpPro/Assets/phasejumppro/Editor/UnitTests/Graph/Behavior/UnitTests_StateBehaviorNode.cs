using UnityEngine;
using System;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_StateBehaviorNode
    {
        private class TestState : Statable<string>
        {
            public string state = "";

            public string State
            {
                get => state;
                set
                {
                    state = value;
                }
            }
        }

        private class TimedStateBehaviorNode : TimedStateBehaviorNode<string>
        {
            public TimedStateBehaviorNode() : base("state", 1.0f)
            {
            }
        }

        [Test]
        public void TestTimedStateBehavior()
        {
            var t1 = new TimedStateBehaviorNode();
            var state = new TestState();
            t1.owner = new WeakReference<object>(state);

            Assert.AreEqual("", state.State);
            t1.Run();
            Assert.IsTrue(t1.IsRunning);
            Assert.AreEqual("state", state.State);

            t1.OnUpdate(new TimeSlice(1.0f));
            Assert.IsFalse(t1.IsRunning);
        }
    }
}
