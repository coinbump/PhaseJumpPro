using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    class UnitTests_Binding
    {
        [Test]
        public void TestBinding()
        {
            var intValue = new PublishedValue<int>(10);
            var stringValue = new PublishedValue<string>("hello");

            Assert.AreEqual("hello", stringValue.Value);
            Assert.AreEqual(10, intValue.Value);

            var binding = new IntToStringBinding(intValue, stringValue);
            Assert.AreEqual("10", stringValue.Value);
            Assert.AreEqual(10, intValue.Value);

            intValue.Value = 5;
            Assert.AreEqual("5", stringValue.Value);
        }
    }
}
