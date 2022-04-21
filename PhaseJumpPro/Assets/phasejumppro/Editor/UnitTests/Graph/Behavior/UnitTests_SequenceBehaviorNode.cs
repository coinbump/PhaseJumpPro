using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_SequenceBehaviorNode
    {
        private class TestConditionBehaviorNode : ConditionBehaviorNode
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

        private class TestTimedBehavior : TimedBehaviorNode
        {
            public TestTimedBehavior()
                : base(1.0f)
            {
            }
        }

        [Test]
        public void TestSequenceBehavior_NotRepeatingFalseTrueFinish()
        {
            var t1 = new SequenceBehaviorNode(false);
            var c1 = new TestConditionBehaviorNode(false);
            var c2 = new TestConditionBehaviorNode(true);
            t1.AddChild(c1);
            t1.AddChild(c2);

            // Condition nodes must have a child for success
            c1.AddChild(new TestSuccessNode());
            c2.AddChild(new TestSuccessNode());

            // False evaluation node is skipped, true evaluation node is run
            t1.Run();
            Assert.AreEqual(1, c1.runCount);
            Assert.AreEqual(1, c2.runCount);

            Assert.IsFalse(t1.IsRunning);
            Assert.IsFalse(c1.IsRunning);
            Assert.IsFalse(c2.IsRunning);
            Assert.AreEqual(BehaviorNode.StateType.Fail, c1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, t1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, c2.State);
        }

        [Test]
        public void TestSequenceBehavior_Running()
        {
            var t1 = new SequenceBehaviorNode(false);
            var c1 = new TimedBehaviorNode(1.0f);
            t1.AddChild(c1);
           
            t1.Run();

            Assert.IsTrue(t1.IsRunning);
            Assert.IsTrue(c1.IsRunning);

            t1.OnUpdate(new TimeSlice(1.0f));

            Assert.False(t1.IsRunning);
            Assert.False(c1.IsRunning);
        }

        [Test]
        public void TestSequenceBehavior_NotRepeatingFalseTrueTrue()
        {
            var t1 = new SequenceBehaviorNode(false);
            var c1 = new TestConditionBehaviorNode(false);
            var c2 = new TestConditionBehaviorNode(true);
            var c3 = new TestConditionBehaviorNode(true);
            t1.AddChild(c1);
            t1.AddChild(c2);
            t1.AddChild(c3);

            c1.AddChild(new TestSuccessNode());
            c2.AddChild(new TestSuccessNode());
            c3.AddChild(new TestSuccessNode());

            t1.Run();
            Assert.AreEqual(1, c1.runCount);
            Assert.AreEqual(1, c2.runCount);
            Assert.AreEqual(0, c3.runCount);

            Assert.IsFalse(t1.IsRunning);
            Assert.IsFalse(c1.IsRunning);
            Assert.IsFalse(c2.IsRunning);
            Assert.AreEqual(BehaviorNode.StateType.Fail, c1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, t1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, c2.State);

            t1.Run();
            Assert.AreEqual(1, c1.runCount);
            Assert.AreEqual(1, c2.runCount);
            Assert.AreEqual(1, c3.runCount);

            Assert.IsFalse(t1.IsRunning);
            Assert.IsFalse(c1.IsRunning);
            Assert.IsFalse(c2.IsRunning);
            Assert.IsFalse(c3.IsRunning);
            Assert.AreEqual(BehaviorNode.StateType.Fail, c1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, t1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, c3.State);

            // Test repeat (should fail, repeat is turned off)
            t1.Run();
            Assert.AreEqual(1, c1.runCount);
            Assert.AreEqual(1, c2.runCount);
            Assert.AreEqual(1, c3.runCount);

            Assert.IsFalse(t1.IsRunning);
            Assert.IsFalse(c1.IsRunning);
            Assert.IsFalse(c2.IsRunning);
            Assert.IsFalse(c3.IsRunning);
            Assert.AreEqual(BehaviorNode.StateType.Fail, c1.State);
            Assert.AreEqual(BehaviorNode.StateType.Fail, t1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, c3.State);
        }

        [Test]
        public void TestSequenceBehavior_RepeatingFalseTrueTrue()
        {
            var t1 = new SequenceBehaviorNode(true);
            var c1 = new TestConditionBehaviorNode(false);
            var c2 = new TestConditionBehaviorNode(true);
            var c3 = new TestConditionBehaviorNode(true);
            t1.AddChild(c1);
            t1.AddChild(c2);
            t1.AddChild(c3);

            c1.AddChild(new TestSuccessNode());
            c2.AddChild(new TestSuccessNode());
            c3.AddChild(new TestSuccessNode());

            t1.Run();
            Assert.AreEqual(1, c1.runCount);
            Assert.AreEqual(1, c2.runCount);
            Assert.AreEqual(0, c3.runCount);

            Assert.IsFalse(t1.IsRunning);
            Assert.IsFalse(c1.IsRunning);
            Assert.IsFalse(c2.IsRunning);
            Assert.AreEqual(BehaviorNode.StateType.Fail, c1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, t1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, c2.State);

            t1.Run();
            Assert.AreEqual(1, c1.runCount);
            Assert.AreEqual(1, c2.runCount);
            Assert.AreEqual(1, c3.runCount);

            Assert.IsFalse(t1.IsRunning);
            Assert.IsFalse(c1.IsRunning);
            Assert.IsFalse(c2.IsRunning);
            Assert.IsFalse(c3.IsRunning);
            Assert.AreEqual(BehaviorNode.StateType.Fail, c1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, t1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, c3.State);

            // Test repeat (should succeed, repeat is turned on)
            t1.Run();
            Assert.AreEqual(2, c1.runCount);
            Assert.AreEqual(2, c2.runCount);
            Assert.AreEqual(1, c3.runCount);

            Assert.IsFalse(t1.IsRunning);
            Assert.IsFalse(c1.IsRunning);
            Assert.IsFalse(c2.IsRunning);
            Assert.IsFalse(c3.IsRunning);
            Assert.AreEqual(BehaviorNode.StateType.Fail, c1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, t1.State);
            Assert.AreEqual(BehaviorNode.StateType.Success, c2.State);
        }
    }
}
