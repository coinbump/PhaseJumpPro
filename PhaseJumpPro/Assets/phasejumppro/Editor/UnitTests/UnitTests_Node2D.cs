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
            var test = gameObject.AddComponent<GoNode2D>();
            test.Rotation = Angle.DegreesAngle(180.0f);
            Assert.AreEqual(test.RotationNormal, .5f);
            test.Rotation = test.Rotation + Angle.DegreesAngle(360.0f);
            Assert.AreEqual(test.RotationNormal, .5f);
        }
    }
}
