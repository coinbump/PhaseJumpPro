using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_BehaviorSequenceBuilder
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

        [Test]
        public void TestBehaviorSequenceBuilder()
        {
            var state = new TestState();
            var builder = new BehaviorSequenceBuilder<string>(false);
            builder.sequenceNode.owner = new WeakReference<object>(state);

            builder
                .State("1")
                .Wait(1.0f)
                .State("hold", 1.0f)
                .State("finish");

            var node = builder.sequenceNode;

            Assert.AreEqual("", state.state);
            node.Run();

            Assert.AreEqual("1", state.state);
            node.OnUpdate(new TimeSlice(1.0f));
            node.OnUpdate(new TimeSlice(1.0f));

            Assert.AreEqual("hold", state.state);
            node.OnUpdate(new TimeSlice(1.0f));
            node.OnUpdate(new TimeSlice(1.0f));

            Assert.AreEqual("finish", state.state);
        }
    }
}
