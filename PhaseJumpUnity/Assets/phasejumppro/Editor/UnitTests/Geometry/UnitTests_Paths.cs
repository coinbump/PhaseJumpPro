using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_Paths
    {
        [Test]
        public void Test_CirclePath()
        {
            var sut = new CirclePath(1.0f);
            Assert.AreEqual(0, sut.PositionAt(0).x, 0.001f);
            Assert.AreEqual(1.0f, sut.PositionAt(0).y, 0.001f);

            Assert.AreEqual(1.0f, sut.PositionAt(90.0f / 360.0f).x);
            Assert.AreEqual(0, sut.PositionAt(90.0f / 360.0f).y, 0.001f);

            Assert.AreEqual(0, sut.PositionAt(180.0f / 360.0f).x, 0.001f);
            Assert.AreEqual(-1.0f, sut.PositionAt(180.0f / 360.0f).y, 0.001f);
        }
    }
}
