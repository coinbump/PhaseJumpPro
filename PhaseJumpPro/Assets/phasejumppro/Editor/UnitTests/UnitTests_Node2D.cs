using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_Node2D
    {
        [Test]
        public void UnitTests()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<GoNode2D>();
            sut.Rotation = Angle.DegreesAngle(180.0f);
            Assert.AreEqual(0.5f, sut.RotationNormal);
            sut.Rotation = sut.Rotation + Angle.DegreesAngle(360.0f);
            Assert.AreEqual(0.5f, sut.RotationNormal);

            sut.Rotation = Angle.DegreesAngle(45.0f);
            Assert.AreEqual(0.125f, sut.RotationNormal, 0.001f);
        }
    }
}
