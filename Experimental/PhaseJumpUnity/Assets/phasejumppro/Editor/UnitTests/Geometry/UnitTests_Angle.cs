using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_Angle
    {
        [Test]
        public void Test_InitZero()
        {
            var sut = Angle.zero;
            Assert.AreEqual(0, sut.Radians);
            Assert.AreEqual(0, sut.Degrees);
        }

        [Test]
        public void Test_InitDegrees()
        {
            var sut = Angle.DegreesAngle(360.0f);
            Assert.AreEqual(Mathf.PI * 2.0f, sut.Radians);
        }

        [Test]
        public void Test_InitRadians()
        {
            var sut = Angle.RadiansAngle(Mathf.PI * 2.0f);
            Assert.AreEqual(360.0f, sut.Degrees);
        }

        [Test]
        public void Test_Clipped()
        {
            Assert.AreEqual(1, Angle.DegreesAngle(361).Clipped().Degrees);
            Assert.AreEqual(0, Angle.DegreesAngle(-360).Clipped().Degrees);
            Assert.AreEqual(359, Angle.DegreesAngle(-361).Clipped().Degrees);
            Assert.AreEqual(0, Angle.DegreesAngle(-720).Clipped().Degrees);
        }

        [Test]
        public void Test_ToVector2()
        {
            float sqrt2 = Mathf.Sqrt(2);

            Vector2 adjOpp = Angle.DegreesAngle(45).ToVector2(sqrt2);
            Assert.AreEqual(adjOpp.x, 1, .01);
            Assert.AreEqual(adjOpp.y, 1 * Vector2.up.y, .01);
            adjOpp = Angle.DegreesAngle(135).ToVector2(sqrt2);
            Assert.AreEqual(adjOpp.x, 1, .01);
            Assert.AreEqual(adjOpp.y, 1 * Vector2.down.y, .01);
            adjOpp = Angle.DegreesAngle(225).ToVector2(sqrt2);
            Assert.AreEqual(adjOpp.x, -1, .01);
            Assert.AreEqual(adjOpp.y, 1 * Vector2.down.y, .01);
            adjOpp = Angle.DegreesAngle(315).ToVector2(sqrt2);
            Assert.AreEqual(adjOpp.x, -1, .01);
            Assert.AreEqual(adjOpp.y, 1 * Vector2.up.y, .01);

            // IMPORTANT: the angle specified is away from the 0 degree, *not* from the x-coordinate line.
            // 60 degrees rotated away from origin 0 is a 30 degree right triangle.
            adjOpp = Angle.DegreesAngle(60).ToVector2(1);
            Assert.AreEqual(adjOpp.x, .866, .01);
            Assert.AreEqual(adjOpp.y, .5 * Vector2.up.y, .01);
        }

        [Test]
        public void Test_FromVector2()
        {
            Vector2 distance;
            Angle angle;
            distance.x = 0;
            distance.y = 0;
            angle = new Angle(distance);
            Assert.AreEqual(0, angle.Degrees);

            distance.x = 0;
            distance.y = 10 * Vector2.up.y;
            angle = new Angle(distance);
            Assert.AreEqual(0, angle.Degrees, .001f);
            distance.x = 10;
            distance.y = 0;
            angle = new Angle(distance);
            Assert.AreEqual(90, angle.Degrees, .001f);
            distance.x = 0;
            distance.y = 10 * Vector2.down.y;
            angle = new Angle(distance);
            Assert.AreEqual(180, angle.Degrees, .001f);
            distance.x = -10;
            distance.y = 0;
            angle = new Angle(distance);
            Assert.AreEqual(270, angle.Degrees, .001f);

            distance.x = 10;
            distance.y = 10 * Vector2.up.y;
            angle = new Angle(distance);
            Assert.AreEqual(45, angle.Degrees, .001f);
            distance.x = 10;
            distance.y = 10 * Vector2.down.y;
            angle = new Angle(distance);
            Assert.AreEqual(135, angle.Degrees, .001f);
            distance.x = -10;
            distance.y = 10 * Vector2.down.y;
            angle = new Angle(distance);
            Assert.AreEqual(225, angle.Degrees, .001f);
            distance.x = -10;
            distance.y = 10 * Vector2.up.y;
            angle = new Angle(distance);
            Assert.AreEqual(315, angle.Degrees, .001f);
        }
    }
}
