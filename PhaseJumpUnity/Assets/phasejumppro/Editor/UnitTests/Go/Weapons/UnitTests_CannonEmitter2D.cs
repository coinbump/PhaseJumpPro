using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;
using PJ;

namespace PJ
{
    public class UnitTests_CannonEmitter2D
    {
        class TestCannonEmitter2D : CannonEmitter2D
        {
            public void SetEmitAngle(Angle angle)
            {
                this.emitAngle = angle;
            }

            public Vector3 TestEmitVelocity(Emit emit) { return EmitVelocity(emit); }
        }

        [Test]
        public void Test_EmitVelocity_PositiveAngles()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestCannonEmitter2D>();

            var emit = new Emit((Vector2)Angle.DegreesAngle(45.0f), Vector3.zero, 0, 0);
            sut.SetEmitAngle(Angle.DegreesAngle(45.0f));

            var emitVelocity = sut.TestEmitVelocity(emit);
            Assert.AreEqual(90.0f, (new Angle((Vector2)emitVelocity)).Degrees, .0001f);
        }

        [Test]
        public void Test_EmitVelocity_PositiveAddNegativeAngle()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestCannonEmitter2D>();

            var emit = new Emit((Vector2)Angle.DegreesAngle(45.0f), Vector3.zero, 0, 0);
            sut.SetEmitAngle(Angle.DegreesAngle(-45.0f));

            var emitVelocity = sut.TestEmitVelocity(emit);
            Assert.AreEqual(0, (new Angle((Vector2)emitVelocity)).Degrees, .0001f);
        }

        [Test]
        public void Test_EmitVelocity_NegativeAddPositiveAngle()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestCannonEmitter2D>();

            var emit = new Emit((Vector2)Angle.DegreesAngle(-45.0f), Vector3.zero, 0, 0);
            sut.SetEmitAngle(Angle.DegreesAngle(45.0f));

            var emitVelocity = sut.TestEmitVelocity(emit);
            Assert.AreEqual(0, (new Angle((Vector2)emitVelocity)).Degrees, .0001f);
        }
    }
}
