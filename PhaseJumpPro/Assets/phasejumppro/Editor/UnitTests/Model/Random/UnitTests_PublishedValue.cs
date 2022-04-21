using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    class UnitTests_PublishedValue
    {
        private class TestValue : PublishedValue<int>, SomeListener
        {
            public TestValue()
            {
            }

            protected override void OnValueChange()
            {
                base.OnValueChange();

                changedCount++;
            }

            public void OnListen(PJ.Event theEvent)
            {
                var valueChangeEvent = theEvent as EventValueChange;
                if (null != valueChangeEvent)
                {
                    OnValueChange();
                }
            }

            public int changedCount;
        }

        [Test]
        public void TestPublishedValue()
        {
            var test = new TestValue();
            Assert.AreEqual(0, test.Value);
            test.Value = 10;
            Assert.AreEqual(1, test.changedCount);
            test.Value = 10;
            Assert.AreEqual(1, test.changedCount);

            test.Value = 11;
            Assert.AreEqual(2, test.changedCount);
        }
    }
}
