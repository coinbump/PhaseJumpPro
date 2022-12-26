using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_AngleUtils
	{
		[Test]
		public void TestAngleUtils()
		{
            float sqrt2 = Mathf.Sqrt(2);
			float distX = 1;
			float distY = 1 * Vector2.down.y;
			float hyp = AngleUtils.Hypotenuse(distX, distY);
			Assert.AreEqual(hyp, sqrt2, .01);
			hyp = AngleUtils.SquaredHypotenuse(distX, distY);
			Assert.AreEqual(hyp, 2, .01);

			Vector2 adjOpp = Angle.DegreesAngle(45).ToVector2(sqrt2);
			Assert.AreEqual(adjOpp.x, 1, .01);
			Assert.AreEqual(adjOpp.y, 1*Vector2.up.y, .01);
			Assert.AreEqual(AngleUtils.AngleOppToHyp(Angle.DegreesAngle(45), 1), sqrt2, .01);

			{
				float firstDeg = 180;
				float finalDeg = 45;
				var turnDeg = Angle.DegreesAngle(firstDeg).MinAngleTo(Angle.DegreesAngle(finalDeg));
				Assert.AreEqual(-135, turnDeg.Degrees, .01);
			}
			{
				float firstDeg = 45;
				float finalDeg = 180;
				var turnDeg = Angle.DegreesAngle(firstDeg).MinAngleTo(Angle.DegreesAngle(finalDeg));
				Assert.AreEqual(135, turnDeg.Degrees, .01);
			}
		}
	}
}
