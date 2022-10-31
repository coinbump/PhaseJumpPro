using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_Angle
	{
		[Test]
		public void TestInitZero()
		{
			var sut = Angle.zero;
			Assert.AreEqual(0, sut.Radians);
			Assert.AreEqual(0, sut.Degrees);
		}

        [Test]
        public void TestInitDegrees()
        {
			var sut = Angle.DegreesAngle(360.0f);
            Assert.AreEqual(Mathf.PI * 2.0f, sut.Radians);
        }

        [Test]
        public void TestInitRadians()
        {
            var sut = Angle.RadiansAngle(Mathf.PI * 2.0f);
            Assert.AreEqual(360.0f, sut.Degrees);
        }
    }
}
