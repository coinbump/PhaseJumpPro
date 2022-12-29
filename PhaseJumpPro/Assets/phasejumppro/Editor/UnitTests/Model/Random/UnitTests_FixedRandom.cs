using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;
using System;

namespace PJ
{
    public class UnitTests_FixedRandom
    {
        [Test]
        public void TestFixedRandom()
        {
            var fixedRandom = new FixedNormalRandom();
            fixedRandom.values.Add(0.5f);
            fixedRandom.values.Add(0.7f);
            fixedRandom.values.Add(0.2f);

            Assert.AreEqual(0.5f, fixedRandom.Value);
            Assert.AreEqual(0.7f, fixedRandom.Value);
            Assert.AreEqual(0.2f, fixedRandom.Value);
            Assert.AreEqual(0.5f, fixedRandom.Value);
        }
    }
}
