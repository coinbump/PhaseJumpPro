using System.Collections;
using NUnit.Framework;
using UnityEngine;

namespace PJ
{
    public class UnitTests_Timeline
    {
        [Test]
        public void Test_AddKeyframes()
        {
            var sut = new Timeline();
            sut.Add(new ValueKeyframe<float>(1.0f, 0));

            Assert.AreEqual(1, sut.Keyframes.Count);
            Assert.AreEqual(1.0f, sut.Keyframes[0].time);

            sut.Add(new ValueKeyframe<float>(2.0f, 0));

            Assert.AreEqual(2, sut.Keyframes.Count);
            Assert.AreEqual(1.0f, sut.Keyframes[0].time);
            Assert.AreEqual(2.0f, sut.Keyframes[1].time);

            sut.Add(new ValueKeyframe<float>(1.5f, 0));

            Assert.AreEqual(3, sut.Keyframes.Count);
            Assert.AreEqual(1.0f, sut.Keyframes[0].time);
            Assert.AreEqual(1.5f, sut.Keyframes[1].time);

            sut.Add(new ValueKeyframe<float>(0f, 0));

            Assert.AreEqual(4, sut.Keyframes.Count);
            Assert.AreEqual(0f, sut.Keyframes[0].time);
            Assert.AreEqual(1.0f, sut.Keyframes[1].time);

            sut.Add(new ValueKeyframe<float>(5.0f, 0));

            Assert.AreEqual(5, sut.Keyframes.Count);
            Assert.AreEqual(0f, sut.Keyframes[0].time);
            Assert.AreEqual(5.0f, sut.Keyframes[4].time);
        }

        [Test]
        public void Test_RemoveInRange()
        {
            var sut = new Timeline();
            sut.Add(new ValueKeyframe<float>(0f, 0));
            sut.Add(new ValueKeyframe<float>(1.0f, 0));
            sut.Add(new ValueKeyframe<float>(1.5f, 0));
            sut.Add(new ValueKeyframe<float>(2.0f, 0));

            sut.RemoveIn(Range<float>.InclusiveRange(1.0f, 1.5f));

            Assert.AreEqual(2, sut.Keyframes.Count);
            Assert.AreEqual(0, sut.Keyframes[0].time);
            Assert.AreEqual(2.0f, sut.Keyframes[1].time);
        }

        [Test]
        public void Test_RemoveAtTime()
        {
            var sut = new Timeline();
            sut.Add(new ValueKeyframe<float>(0f, 0));
            sut.Add(new ValueKeyframe<float>(1.0f, 0));
            sut.Add(new ValueKeyframe<float>(1.5f, 0));
            sut.Add(new ValueKeyframe<float>(2.0f, 0));

            sut.RemoveAt(0);
            sut.RemoveAt(1.5f);

            Assert.AreEqual(2, sut.Keyframes.Count);
            Assert.AreEqual(1.0f, sut.Keyframes[0].time);
            Assert.AreEqual(2.0f, sut.Keyframes[1].time);
        }

        [Test]
        public void Test_KeyframeBefore()
        {
            var sut = new Timeline();
            sut.Add(new ValueKeyframe<float>(0f, 0));
            sut.Add(new ValueKeyframe<float>(1.0f, 0));
            sut.Add(new ValueKeyframe<float>(1.5f, 0));
            sut.Add(new ValueKeyframe<float>(2.0f, 0));

            Assert.AreEqual(null, sut.KeyframeBefore(0));
            Assert.AreEqual(0, sut.KeyframeBefore(1.0f).time);
            Assert.AreEqual(1.0f, sut.KeyframeBefore(1.1f).time);
            Assert.AreEqual(2.0f, sut.KeyframeBefore(5.0f).time);
        }

        [Test]
        public void Test_KeyframeAfterOrAt()
        {
            var sut = new Timeline();
            sut.Add(new ValueKeyframe<float>(0f, 0));
            sut.Add(new ValueKeyframe<float>(1.0f, 0));
            sut.Add(new ValueKeyframe<float>(1.5f, 0));
            sut.Add(new ValueKeyframe<float>(2.0f, 0));

            Assert.AreEqual(0, sut.KeyframeAfterOrAt(0).time);
            Assert.AreEqual(1.0f, sut.KeyframeAfterOrAt(1.0f).time);
            Assert.AreEqual(1.5f, sut.KeyframeAfterOrAt(1.06f).time);
            Assert.AreEqual(null, sut.KeyframeAfterOrAt(3.0f));
        }

        [Test]
        public void Test_KeyframesIn()
        {
            var sut = new Timeline();
            sut.Add(new ValueKeyframe<float>(0f, 0));
            sut.Add(new ValueKeyframe<float>(1.0f, 0));
            sut.Add(new ValueKeyframe<float>(1.5f, 0));
            sut.Add(new ValueKeyframe<float>(2.0f, 0));

            var keyframes = sut.KeyframesIn(Range<float>.InclusiveRange(1.0f, 1.5f));

            Assert.AreEqual(2, keyframes.Count);
            Assert.AreEqual(1.0f, keyframes[0].time);
            Assert.AreEqual(1.5f, keyframes[1].time);
        }
    }
}
