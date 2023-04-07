using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;
using System;

namespace PJ
{
    public class UnitTests_SomeRandomChoice
    {
        [Test]
        public void TestMinMaxInt()
        {
            var fixedRandom = new FixedRandom();
            fixedRandom.values.Add(0.0f);
            fixedRandom.values.Add(0.5f);
            fixedRandom.values.Add(1.0f);

            var sut = new MinMaxIntRandomChoice(0, 10);
            Assert.AreEqual(0, sut.Choose(fixedRandom));
            Assert.AreEqual(5, sut.Choose(fixedRandom));
            Assert.AreEqual(10, sut.Choose(fixedRandom));
        }

        [Test]
        public void TestMinMaxFloat()
        {
            var fixedRandom = new FixedRandom();
            fixedRandom.values.Add(0.0f);
            fixedRandom.values.Add(0.5f);
            fixedRandom.values.Add(1.0f);

            var sut = new MinMaxFloatRandomChoice(0.0f, 10.0f);
            Assert.AreEqual(0, sut.Choose(fixedRandom));
            Assert.AreEqual(5.0f, sut.Choose(fixedRandom));
            Assert.AreEqual(10.0f, sut.Choose(fixedRandom));
        }
    }
}
