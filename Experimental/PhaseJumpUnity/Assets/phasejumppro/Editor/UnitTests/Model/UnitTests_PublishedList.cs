using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    class UnitTests_PublishedList
    {
        private class TestValue : PublishedList<int>
        {
            public int changedCount;

            public TestValue()
            {
            }

            protected override void OnValueChange()
            {
                base.OnValueChange();

                changedCount++;
            }
        }

        [Test]
        public void TestPublishedList()
        {
            var test = new TestValue();
            Assert.AreEqual(0, test.Value.Count);

            test.Value.Add(10);
            Assert.AreEqual(1, test.changedCount);
            Assert.AreEqual(1, test.Value.Count);
            Assert.AreEqual(10, test.Value[0]);

            test.Value.Add(11);
            Assert.AreEqual(2, test.changedCount);
            Assert.AreEqual(2, test.Value.Count);
            Assert.AreEqual(10, test.Value[0]);
            Assert.AreEqual(11, test.Value[1]);

            test.Value.RemoveAt(0);
            Assert.AreEqual(3, test.changedCount);

            test.Value.Clear();
            Assert.AreEqual(4, test.changedCount);
        }
    }
}
