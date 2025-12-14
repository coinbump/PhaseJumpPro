using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_ValueTargetGoal
    {
        [Test]
        public void Test_ValueTargetGoal()
        {
            var sut = new ValueTargetGoal("", 2);

            Assert.AreEqual(0, sut.Progress);

            sut.Increment();
            Assert.IsFalse(sut.IsComplete);
            Assert.AreEqual(1, sut.Value);
            Assert.AreEqual(0.5f, sut.Progress);

            sut.Increment();
            Assert.IsTrue(sut.IsComplete);
            Assert.AreEqual(1.0f, sut.Progress);
        }
    }
}
