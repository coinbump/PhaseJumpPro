using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    class UnitTests_PublishedValue
    {
        private class TestValue<T> : PublishedValue<T> where T : IEquatable<T>
        {
            public TestValue()
            {
            }

            protected override void OnValueChange()
            {
                base.OnValueChange();

                changedCount++;
            }

            public int changedCount;
        }

        class TestClass : IEquatable<TestClass>
        {
            bool IEquatable<TestClass>.Equals(TestClass other)
            {
                return true;
            }
        }

        [Test]
        public void TestPublishedValue()
        {
            var test = new TestValue<int>();
            Assert.AreEqual(0, test.Value);
            test.Value = 10;
            Assert.AreEqual(1, test.changedCount);
            test.Value = 10;
            Assert.AreEqual(1, test.changedCount);

            test.Value = 11;
            Assert.AreEqual(2, test.changedCount);
        }

        [Test]
        public void TestNullChange()
        {
            var test = new TestValue<TestClass>();
            Assert.AreEqual(null, test.Value);
            test.Value = new();
            Assert.AreEqual(1, test.changedCount);
            test.Value = new();
            Assert.AreEqual(1, test.changedCount);

            test.Value = null;
            Assert.AreEqual(2, test.changedCount);
        }

        [Test]
        public void TestTransform()
        {
            var test = new TestValue<int>();
            var transform = new IntClamp(0, 10);
            test.transform = transform;

            Assert.AreEqual(0, test.Value);
            test.Value = 5;
            Assert.AreEqual(5, test.Value);
            test.Value = -1;
            Assert.AreEqual(0, test.Value);
            test.Value = 100;
            Assert.AreEqual(10, test.Value);
        }
    }
}
