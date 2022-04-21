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

			Assert.AreEqual(0, limiter.LimitAngle(0));
			Assert.AreEqual(0, limiter.LimitAngle(44));
			Assert.AreEqual(0, limiter.LimitAngle(-44));

			Assert.AreEqual(90, limiter.LimitAngle(46));
			Assert.AreEqual(90, limiter.LimitAngle(90));
			Assert.AreEqual(90, limiter.LimitAngle(134));

			Assert.AreEqual(180, limiter.LimitAngle(136));
			Assert.AreEqual(180, limiter.LimitAngle(180));
			Assert.AreEqual(180, limiter.LimitAngle(214));

			Assert.AreEqual(270, limiter.LimitAngle(226));
			Assert.AreEqual(270, limiter.LimitAngle(270));
			Assert.AreEqual(270, limiter.LimitAngle(314));

			Assert.AreEqual(360, limiter.LimitAngle(316));
			Assert.AreEqual(360, limiter.LimitAngle(360));
		}
	}
}
