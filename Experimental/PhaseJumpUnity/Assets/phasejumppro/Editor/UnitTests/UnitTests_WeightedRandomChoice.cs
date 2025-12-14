using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    class UnitTests_WeightedRandomChoice
    {
        [Test]
        public void UnitTests()
        {
            var sut = new WeightedRandomChoice<string>();
            var w1 = new Weight<string>(1.0f, "1");
            var w2 = new Weight<string>(0.5f, "2");
            var w3 = new Weight<string>(0.25f, "3");
            var totalWeight = w1.WeightValue + w2.WeightValue + w3.WeightValue;

            sut.Add(w1);
            sut.Add(w2);
            sut.Add(w3);

            Assert.AreEqual("1", sut.ChooseWeightAt(.5f / totalWeight).value);
            Assert.AreEqual("1", sut.ChooseWeightAt(.9f / totalWeight).value);
            Assert.AreEqual("2", sut.ChooseWeightAt(1.1f / totalWeight).value);
            Assert.AreEqual("3", sut.ChooseWeightAt(1.6f / totalWeight).value);
        }
    }
}
