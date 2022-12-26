using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_AngleAxisLimiter2D
	{
		[Test]
		public void TestAngleAxisLimiter_4Way()
		{
			var limiter = new AngleAxisLimiter2D(4);

			Assert.AreEqual(0, limiter.LimitAngle(Angle.DegreesAngle(0)).Degrees);
			Assert.AreEqual(0, limiter.LimitAngle(Angle.DegreesAngle(44)).Degrees);
			Assert.AreEqual(0, limiter.LimitAngle(Angle.DegreesAngle(-44)).Degrees);

			Assert.AreEqual(90, limiter.LimitAngle(Angle.DegreesAngle(46)).Degrees);
			Assert.AreEqual(90, limiter.LimitAngle(Angle.DegreesAngle(90)).Degrees);
			Assert.AreEqual(90, limiter.LimitAngle(Angle.DegreesAngle(134)).Degrees);

			Assert.AreEqual(180, limiter.LimitAngle(Angle.DegreesAngle(136)).Degrees);
			Assert.AreEqual(180, limiter.LimitAngle(Angle.DegreesAngle(180)).Degrees);
			Assert.AreEqual(180, limiter.LimitAngle(Angle.DegreesAngle(214)).Degrees);

			Assert.AreEqual(270, limiter.LimitAngle(Angle.DegreesAngle(226)).Degrees);
			Assert.AreEqual(270, limiter.LimitAngle(Angle.DegreesAngle(270)).Degrees);
			Assert.AreEqual(270, limiter.LimitAngle(Angle.DegreesAngle(314)).Degrees);

			Assert.AreEqual(360, limiter.LimitAngle(Angle.DegreesAngle(316)).Degrees);
			Assert.AreEqual(360, limiter.LimitAngle(Angle.DegreesAngle(360)).Degrees);
		}
	}
}
