using UnityEngine;
using System.Collections;
using NUnit.Framework;
using System;

namespace PJ
{
    public class UnitTests_Tags
    {
        [Test]
        public void TestSafeValue()
        {
            Func<string> newString = delegate () { return string.Empty; };

            var sut = new Tags();
            Assert.AreEqual(sut.SafeValue<int>("test-int", -1), -1);
            Assert.AreEqual(sut.SafeValue<string>("test-string", newString), "");

            sut.Add("test-string-add", "add");
            Assert.AreEqual(sut.SafeValue<string>("test-string-add", newString), "add");

            sut["foo"] = 10;
            Assert.AreEqual(sut.SafeValue<int>("foo", 0), 10);
        }

        [Test]
        public void TestValueTypesConverted()
        {
            var sut = new Tags();
            Int64 int64Value = 10;
            sut["test"] = int64Value;

            Assert.AreEqual(10, sut.SafeValue<Int64>("test", 0));
            Assert.AreEqual(0, sut.SafeValue<int>("test", 0));

            sut = sut.ValueTypesConverted<Int64, int>((left) => Convert.ToInt32(left));

            Assert.AreEqual(10, sut.SafeValue<int>("test", 0));
        }

        [Test]
        public void TestSetOptionalValue()
        {
            var sut = new Tags();

            Assert.AreEqual(0, sut.SafeValue<int>("test", 0));

            sut.SetOptionalValue("test", new Optional<int>(5));
            Assert.AreEqual(5, sut.SafeValue<int>("test", 0));

            sut.SetOptionalValue<int>("test", null);
            Assert.AreEqual(0, sut.SafeValue<int>("test", 0));
        }
    }
}
