using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_SelectorBehaviorNode
    {
        private class TestConditionBehaviorNode : SomeConditionBehaviorNode
        {
            public bool evaluate;
            public int runCount;

            public TestConditionBehaviorNode()
            {
            }

            public TestConditionBehaviorNode(bool evaluate)
            {
                this.evaluate = evaluate;
            }

            public override bool EvaluateCondition()
            {
                return evaluate;
            }

            protected override StateType Evaluate()
            {
                runCount++;
                return base.Evaluate();
            }
        }

        private class TestTimedBehaviorNode : TimedBehaviorNode
        {
            public TestTimedBehaviorNode()
                : base(1.0f)
            {
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

        [Test]
        public void TestSelectorBehavior()
        {
            var t1 = new SelectorBehaviorNode();
            var c1 = new TestConditionBehaviorNode(false);
            var c2 = new TestConditionBehaviorNode(true);
            var c3 = new TestConditionBehaviorNode(true);
            t1.AddChild(c1);
            t1.AddChild(c2);
            t1.AddChild(c3);

            // Condition nodes must have a child for success
            c1.AddChild(new TestSuccessNode());
            c2.AddChild(new TestSuccessNode());
            c3.AddChild(new TestSuccessNode());

            t1.Run();
            Assert.AreEqual(1, c1.runCount);
            Assert.AreEqual(1, c2.runCount);
            Assert.AreEqual(0, c3.runCount);
            Assert.IsFalse(c1.IsRunning);
            Assert.IsFalse(c2.IsRunning);
            Assert.AreEqual(BehaviorNode.StateType.Success, t1.State);
            Assert.AreEqual(BehaviorNode.StateType.Fail, c1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, c2.State);

            t1.Run();
            Assert.AreEqual(2, c1.runCount);
            Assert.AreEqual(2, c2.runCount);
            Assert.AreEqual(0, c3.runCount);
            Assert.IsFalse(c1.IsRunning);
            Assert.IsFalse(c2.IsRunning);
            Assert.AreEqual(BehaviorNode.StateType.Success, t1.State);
            Assert.AreEqual(BehaviorNode.StateType.Fail, c1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, c2.State);

            c2.evaluate = false;

            t1.Run();
            Assert.AreEqual(3, c1.runCount);
            Assert.AreEqual(3, c2.runCount);
            Assert.AreEqual(1, c3.runCount);
            Assert.IsFalse(c1.IsRunning);
            Assert.IsFalse(c2.IsRunning);
            Assert.AreEqual(BehaviorNode.StateType.Success, t1.State);
            Assert.AreEqual(BehaviorNode.StateType.Fail, c1.State);
            Assert.AreEqual(BehaviorNode.StateType.Fail, c2.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, c3.State);
        }
    }
}
