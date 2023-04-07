using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_GetProtectedSet
    {
        private class TestCounter
        {
            public static int counter = 0;

            public TestCounter()
            {
                counter++;
            }
        }

        private class Test
        {
            public TestCounter value { get; protected set; } = new TestCounter();
        }

        [Test]
        public void Test_GetProtectedSet()
        {
            TestCounter.counter = 0;

            var test = new Test();
            var value1 = test.value;

            Assert.AreEqual(1, TestCounter.counter);

            var value2 = test.value;

            Assert.AreEqual(1, TestCounter.counter);
        }
    }
}

