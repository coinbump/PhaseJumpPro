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
			float distY = 1;
			float hyp = AngleUtils.GetHypotenuse(distX, distY);
			Assert.AreEqual(hyp, sqrt2, .01);
			hyp = AngleUtils.GetSquaredHypotenuse(distX, distY);
			Assert.AreEqual(hyp, 2, .01);

			Vector2 adjOpp = AngleUtils.DegTurnAngleHypToDistance(45, sqrt2);
			Assert.AreEqual(adjOpp.x, 1, .01);
			Assert.AreEqual(adjOpp.y, -1, .01);
			Assert.AreEqual(AngleUtils.DegAngleOppToHyp(45, 1), sqrt2, .01);
			adjOpp = AngleUtils.DegTurnAngleHypToDistance(135, sqrt2);
			Assert.AreEqual(adjOpp.x, 1, .01);
			Assert.AreEqual(adjOpp.y, 1, .01);
			adjOpp = AngleUtils.DegTurnAngleHypToDistance(225, sqrt2);
			Assert.AreEqual(adjOpp.x, -1, .01);
			Assert.AreEqual(adjOpp.y, 1, .01);
			adjOpp = AngleUtils.DegTurnAngleHypToDistance(315, sqrt2);
			Assert.AreEqual(adjOpp.x, -1, .01);
			Assert.AreEqual(adjOpp.y, -1, .01);

			// IMPORTANT: the angle specified is away from the 0 degree, *not* from the x-coordinate line.
			// 60 degrees rotated away from origin 0 is a 30 degree right triangle.
			adjOpp = AngleUtils.DegTurnAngleHypToDistance(60, 1);
			Assert.AreEqual(adjOpp.x, .866, .01);
			Assert.AreEqual(adjOpp.y, -.5, .01);

			{
				float firstDeg = 180;
				float finalDeg = 45;
				float turnDeg = AngleUtils.GetClosestDegTurn(firstDeg, finalDeg);
				Assert.AreEqual(-135, turnDeg, .01);
			}
			{
				float firstDeg = 45;
				float finalDeg = 180;
				float turnDeg = AngleUtils.GetClosestDegTurn(firstDeg, finalDeg);
				Assert.AreEqual(135, turnDeg, .01);
			}

			Vector2 distance;
			float angle;
			distance.x = 0;
			distance.y = 0;
			angle = AngleUtils.DistanceToDegTurnAngle(distance);
			Assert.AreEqual(0, angle);

			distance.x = 0;
			distance.y = -10;
			angle = AngleUtils.DistanceToDegTurnAngle(distance);
			Assert.AreEqual(0, angle);
			distance.x = 10;
			distance.y = 0;
			angle = AngleUtils.DistanceToDegTurnAngle(distance);
			Assert.AreEqual(90, angle);
			distance.x = 0;
			distance.y = 10;
			angle = AngleUtils.DistanceToDegTurnAngle(distance);
			Assert.AreEqual(180, angle);
			distance.x = -10;
			distance.y = 0;
			angle = AngleUtils.DistanceToDegTurnAngle(distance);
			Assert.AreEqual(270, angle);

			distance.x = 10;
			distance.y = -10;
			angle = AngleUtils.DistanceToDegTurnAngle(distance);
			Assert.AreEqual(45, angle, .01);
			distance.x = 10;
			distance.y = 10;
			angle = AngleUtils.DistanceToDegTurnAngle(distance);
			Assert.AreEqual(135, angle, .01);
			distance.x = -10;
			distance.y = 10;
			angle = AngleUtils.DistanceToDegTurnAngle(distance);
			Assert.AreEqual(225, angle, .01);
			distance.x = -10;
			distance.y = -10;
			angle = AngleUtils.DistanceToDegTurnAngle(distance);
			Assert.AreEqual(315, angle, .01);
		}
	}
}
