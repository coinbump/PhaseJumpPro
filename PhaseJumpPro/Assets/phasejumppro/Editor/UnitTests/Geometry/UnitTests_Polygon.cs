using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_Polygon
    {
        private class TestPolygon : PJ.Polygon
        {
            public TestPolygon()
            {
                vertices.Add(new Vector3(0, 1));
                vertices.Add(new Vector3(1, 1));
                vertices.Add(new Vector3(1, 0));
                vertices.Add(new Vector3(0, 0));
            }
        }

        [Test]
        public void TestMinMax()
        {
            var sut = new TestPolygon();
            Assert.AreEqual(new Vector3(0, 0, 0), sut.Min);
            Assert.AreEqual(new Vector3(1, 1, 0), sut.Max);
        }

        [Test]
        public void TestSizeCenter()
        {
            var sut = new TestPolygon();
            Assert.AreEqual(new Vector3(1, 1, 0), sut.Size);
            Assert.AreEqual(new Vector3(0.5f, 0.5f, 0), sut.Center);
        }
    }
}
