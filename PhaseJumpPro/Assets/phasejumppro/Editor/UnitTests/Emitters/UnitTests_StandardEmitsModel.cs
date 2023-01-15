using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;
using PJ;

namespace PJ
{
    public class UnitTests_StandardEmitsModel
    {
        [Test]
        public void Test_BuildSpread2D_WithOneAngle()
        {
            var sut = new StandardEmitsModel();
            sut.BuildSpread2D(1, Angle.DegreesAngle(0), Angle.DegreesAngle(0));

            Assert.AreEqual(1, sut.models.Count);
            Assert.AreEqual(0, new Angle((Vector2)sut.models[0].velocity).Degrees);
        }

        [Test]
        public void Test_BuildSpread2D_WithEvenAngles()
        {
            var sut = new StandardEmitsModel();
            sut.BuildSpread2D(2, Angle.DegreesAngle(30), Angle.DegreesAngle(0));

            Assert.AreEqual(2, sut.models.Count);
            Assert.AreEqual(360.0f - 15.0f, new Angle((Vector2)sut.models[0].velocity).Degrees);
            Assert.AreEqual(15.0f, new Angle((Vector2)sut.models[1].velocity).Degrees);
        }

        [Test]
        public void Test_BuildSpread2D_WithOddAngles()
        {
            var sut = new StandardEmitsModel();
            sut.BuildSpread2D(3, Angle.DegreesAngle(30), Angle.DegreesAngle(0));

            Assert.AreEqual(3, sut.models.Count);
            Assert.AreEqual(360.0f - 30.0f, new Angle((Vector2)sut.models[0].velocity).Degrees, .0001f);
            Assert.AreEqual(0, new Angle((Vector2)sut.models[1].velocity).Degrees);
            Assert.AreEqual(30.0f, new Angle((Vector2)sut.models[2].velocity).Degrees, .0001f);
        }

        [Test]
        public void Test_SetVelocityMagnitude()
        {
            var sut = new StandardEmitsModel();
            sut.BuildSpread2D(2, Angle.DegreesAngle(30), Angle.DegreesAngle(0));
            var velocity = 10.0f;
            sut.VelocityMagnitude = velocity;

            Assert.AreEqual(velocity, ((Vector2)sut.models[0].velocity).magnitude, .0001f);
            Assert.AreEqual(velocity, ((Vector2)sut.models[1].velocity).magnitude, .0001f);
        }
    }
}
