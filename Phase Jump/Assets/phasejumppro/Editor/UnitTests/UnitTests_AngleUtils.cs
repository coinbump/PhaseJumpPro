using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_AngleUtils
	{
		[Test]
		public void UnitTests()
		{
			float sqrt2 = Mathf.Sqrt(2);
			float distX = 1;
			float distY = 1*Vector2.down.y;
			float hyp = AngleUtils.Hypotenuse(distX, distY);
			Assert.AreEqual(hyp, sqrt2, .01);
			hyp = AngleUtils.SquaredHypotenuse(distX, distY);
			Assert.AreEqual(hyp, 2, .01);

			Vector2 adjOpp = AngleUtils.DegreeAngleToVector2(45, sqrt2);
			Assert.AreEqual(adjOpp.x, 1, .01);
			Assert.AreEqual(adjOpp.y, 1*Vector2.up.y, .01);
			Assert.AreEqual(AngleUtils.DegAngleOppToHyp(45, 1), sqrt2, .01);
			adjOpp = AngleUtils.DegreeAngleToVector2(135, sqrt2);
			Assert.AreEqual(adjOpp.x, 1, .01);
			Assert.AreEqual(adjOpp.y, 1 * Vector2.down.y, .01);
			adjOpp = AngleUtils.DegreeAngleToVector2(225, sqrt2);
			Assert.AreEqual(adjOpp.x, -1, .01);
			Assert.AreEqual(adjOpp.y, 1 * Vector2.down.y, .01);
			adjOpp = AngleUtils.DegreeAngleToVector2(315, sqrt2);
			Assert.AreEqual(adjOpp.x, -1, .01);
			Assert.AreEqual(adjOpp.y, 1 * Vector2.up.y, .01);

			// IMPORTANT: the angle specified is away from the 0 degree, *not* from the x-coordinate line.
			// 60 degrees rotated away from origin 0 is a 30 degree right triangle.
			adjOpp = AngleUtils.DegreeAngleToVector2(60, 1);
			Assert.AreEqual(adjOpp.x, .866, .01);
			Assert.AreEqual(adjOpp.y, .5*Vector2.up.y, .01);

			{
				float firstDeg = 180;
				float finalDeg = 45;
				float turnDeg = AngleUtils.GetClosestDegreeTurn(firstDeg, finalDeg);
				Assert.AreEqual(-135, turnDeg, .01);
			}
			{
				float firstDeg = 45;
				float finalDeg = 180;
				float turnDeg = AngleUtils.GetClosestDegreeTurn(firstDeg, finalDeg);
				Assert.AreEqual(135, turnDeg, .01);
			}

			Vector2 distance;
			float angle;
			distance.x = 0;
			distance.y = 0;
			angle = AngleUtils.Vector2ToDegreeAngle(distance);
			Assert.AreEqual(0, angle);

			distance.x = 0;
			distance.y = 10*Vector2.up.y;
			angle = AngleUtils.Vector2ToDegreeAngle(distance);
			Assert.AreEqual(0, angle, .001f);
			distance.x = 10;
			distance.y = 0;
			angle = AngleUtils.Vector2ToDegreeAngle(distance);
			Assert.AreEqual(90, angle, .001f);
			distance.x = 0;
			distance.y = 10*Vector2.down.y;
			angle = AngleUtils.Vector2ToDegreeAngle(distance);
			Assert.AreEqual(180, angle, .001f);
			distance.x = -10;
			distance.y = 0;
			angle = AngleUtils.Vector2ToDegreeAngle(distance);
			Assert.AreEqual(270, angle, .001f);

			distance.x = 10;
			distance.y = 10 * Vector2.up.y;
			angle = AngleUtils.Vector2ToDegreeAngle(distance);
			Assert.AreEqual(45, angle, .001f);
			distance.x = 10;
			distance.y = 10 * Vector2.down.y;
			angle = AngleUtils.Vector2ToDegreeAngle(distance);
			Assert.AreEqual(135, angle, .001f);
			distance.x = -10;
			distance.y = 10 * Vector2.down.y;
			angle = AngleUtils.Vector2ToDegreeAngle(distance);
			Assert.AreEqual(225, angle, .001f);
			distance.x = -10;
			distance.y = 10 * Vector2.up.y;
			angle = AngleUtils.Vector2ToDegreeAngle(distance);
			Assert.AreEqual(315, angle, .001f);
		}
	}
}
