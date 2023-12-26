using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_AnimatorTimeline
    {
        [Test]
        public void Test_Expression()
        {
            float value = 0;
            var sut = new AnimatorTimeline<float>(
                new FloatValueInterpolator(),
                new Binding<float>(() => 0, (float newValue) => value = newValue),
                (float progress) => 360.0f * progress
            );
            sut.duration = 1.0f;

            Assert.AreEqual(0, value);
            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(180.0f, value);

            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(360.0f, value);

            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(360.0f, value);
        }

        [Test]
        public void Test_Keyframes()
        {
            float value = 0;
            var sut = new AnimatorTimeline<float>(
                new FloatValueInterpolator(),
                new Binding<float>(() => 0, (float newValue) => value = newValue),
                (float progress) => 360.0f * progress
            );
            sut.duration = 1.0f;
            sut.Add(new ValueKeyframe<float>(0, 0));
            sut.Add(new ValueKeyframe<float>(1.0f, 360.0f));

            Assert.AreEqual(0, value);
            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(180.0f, value);

            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(360.0f, value);

            value = 10.0f;

            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(360.0f, value);
        }
    }
}
