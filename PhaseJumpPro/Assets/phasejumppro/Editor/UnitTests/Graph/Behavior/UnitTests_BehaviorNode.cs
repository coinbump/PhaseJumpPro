using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_BehaviorNode
    {
        private class TestConditionBehaviorNode : ConditionBehaviorNode
        {
            public bool evaluate;

            public TestConditionBehaviorNode(bool evaluate)
            {
                this.evaluate = evaluate;
            }

            public override bool EvaluateCondition()
            {
                return evaluate;
            }
        }

        private class TestFailNode : BehaviorNode
        {
            protected override StateType Evaluate()
            {
                return StateType.Fail;
            }
        }

        private class TestSuccessNode : BehaviorNode
        {
            protected override StateType Evaluate()
            {
                return StateType.Success;
            }
        }

        private class TestTimedBehaviorNode : TimedBehaviorNode
        {
            public TestTimedBehaviorNode()
                : base(1.0f)
            {

            }
        }

        [Test]
        public void TestConditionBehaviorNode_SuccessChild()
        {
            var t1 = new BehaviorNode();
            var c1 = new TestConditionBehaviorNode(false);

            t1.AddChild(c1);
            Assert.AreEqual(c1.owner, t1.owner);

            // Condition evaluates to false -> Fail
            t1.Run();
            Assert.AreEqual(BehaviorNode.StateType.Fail, c1.State);

            // Condition evaluates to true, but no children -> Fail
            c1.evaluate = true;
            t1.Run();
            Assert.AreEqual(BehaviorNode.StateType.Fail, c1.State);

            // Condition evaluates to true, with success child -> Success
            c1.AddChild(new TestSuccessNode());
            t1.Run();
            Assert.AreEqual(BehaviorNode.StateType.Success, c1.State);
        }

        [Test]
        public void TestConditionBehaviorNode_FailChild()
        {
            var t1 = new BehaviorNode();
            var c1 = new TestConditionBehaviorNode(true);

            t1.AddChild(c1);

            // Condition evaluates to true, with fail child -> Fail
            c1.AddChild(new TestFailNode());
            t1.Run();
            Assert.AreEqual(BehaviorNode.StateType.Fail, c1.State);
        }

        [Test]
        public void TestRunning()
        {
            var root = new BehaviorNode(this);
            var t1 = new BehaviorNode();
            var c1 = new TestConditionBehaviorNode(false);
            var c11 = new TestTimedBehaviorNode();
            var c12 = new BehaviorNode();

            root.AddChild(t1);
            t1.AddChild(c1);
            Assert.AreEqual(c1.owner, t1.owner);

            c1.evaluate = true;

            c1.AddChild(c11);
            c1.AddChild(c12);

            Assert.AreEqual(root, t1.RootNode);
            Assert.AreEqual(root, c1.RootNode);
            Assert.AreEqual(root, c11.RootNode);
            Assert.AreEqual(root, c12.RootNode);

            t1.Run();
            Assert.IsTrue(t1.IsRunning);
            Assert.IsTrue(c1.IsRunning);
            Assert.IsTrue(c11.IsRunning);
            Assert.IsFalse(c12.IsRunning);

            t1.OnUpdate(new TimeSlice(1.0f));

            Assert.IsFalse(t1.IsRunning);
            Assert.IsFalse(c1.IsRunning);
            Assert.IsFalse(c11.IsRunning);
        }
    }
}
