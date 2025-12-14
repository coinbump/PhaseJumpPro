using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    class UnitTests_Valve
    {
        private class TestValve : Valve
        {
            public TestValve(Optional<float> onOffDuration)
            {
                turnOnTimer = new TransformTimer(1.0f, SomeRunner.RunType.RunOnce, new LinearInterpolate());
                turnOffTimer = new TransformTimer(1.0f, SomeRunner.RunType.RunOnce, new LinearInterpolate());
                OnStateDuration = onOffDuration;
                OffStateDuration = onOffDuration;
            }
        }

        [Test]
        public void TestValve_WithoutOnOffDuration()
        {
            var sut = new TestValve(null);
            Assert.AreEqual(Valve.StateType.Off, sut.State);
            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(Valve.StateType.Off, sut.State);

            sut.TurnOff(InputEffect.Immediate);
            Assert.AreEqual(Valve.StateType.Off, sut.State);
            Assert.AreEqual(0, sut.ValveState);

            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(Valve.StateType.Off, sut.State);

            sut.TurnOn(InputEffect.Timed);
            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(.5f, sut.ValveState);
            Assert.AreEqual(Valve.StateType.TurnOn, sut.State);
            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(Valve.StateType.On, sut.State);

            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(Valve.StateType.On, sut.State);

            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(Valve.StateType.On, sut.State);

            sut.TurnOff(InputEffect.Timed);
            Assert.AreEqual(Valve.StateType.TurnOff, sut.State);
            Assert.AreEqual(1.0f, sut.ValveState);

            sut.OnUpdate(new TimeSlice(0.3f));
            Assert.AreEqual(.7f, sut.ValveState);
            Assert.AreEqual(Valve.StateType.TurnOff, sut.State);

            sut.TurnOff(InputEffect.Immediate);
            Assert.AreEqual(Valve.StateType.Off, sut.State);
            Assert.AreEqual(0, sut.ValveState);
        }

        [Test]
        public void TestValve_WithOnOffDuration()
        {
            var sut = new TestValve(new Optional<float>(1.0f));
            Assert.AreEqual(Valve.StateType.Off, sut.State);
            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(Valve.StateType.TurnOn, sut.State);

            sut.TurnOff(InputEffect.Immediate);
            Assert.AreEqual(Valve.StateType.Off, sut.State);
            Assert.AreEqual(0, sut.ValveState);

            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(Valve.StateType.TurnOn, sut.State);

            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(.5f, sut.ValveState);
            Assert.AreEqual(Valve.StateType.TurnOn, sut.State);
            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(Valve.StateType.On, sut.State);

            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(Valve.StateType.TurnOff, sut.State);

            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(.5f, sut.ValveState);
            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(Valve.StateType.Off, sut.State);

            sut.OffStateDuration = new Optional<float>(-1.0f);
            sut.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(Valve.StateType.Off, sut.State);

            sut.OffStateDuration = new Optional<float>(1.0f);
            sut.TurnOn(InputEffect.Timed);
            sut.OnUpdate(new TimeSlice(.7f));
            Assert.AreEqual(Valve.StateType.TurnOn, sut.State);

            sut.TurnOff(InputEffect.Timed);
            Assert.AreEqual(Valve.StateType.TurnOff, sut.State);
            Assert.AreEqual(.7f, sut.ValveState);

            sut.TurnOff(InputEffect.Immediate);
            Assert.AreEqual(Valve.StateType.Off, sut.State);
            Assert.AreEqual(0, sut.ValveState);
        }
    }
}
