using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_SegmentState
	{
		[Test]
		public void TestInit()
		{
			var sut = new SegmentState<string>("i");

			Assert.AreEqual("i", sut.state.Value);

			sut.segments.Add(new SegmentState<string>.Segment(1.0f, "p"));
			Assert.AreEqual(1.0f, sut.Size);
        }

        [Test]
        public void TestSize()
        {
            var sut = new SegmentState<string>("i");

            Assert.AreEqual("i", sut.state.Value);

            sut.segments.Add(new SegmentState<string>.Segment(1.0f, "p"));
            sut.segments.Add(new SegmentState<string>.Segment(1.0f, "p"));
            Assert.AreEqual(2.0f, sut.Size);
        }

        [Test]
        public void TestStateForZeroOne()
        {
            var sut = new SegmentState<string>("i");

            Assert.AreEqual("i", sut.state.Value);

            sut.segments.Add(new SegmentState<string>.Segment(1.0f, "p"));
            Assert.AreEqual(1.0f, sut.Size);

            sut.NormalValue = 0;
            Assert.AreEqual("p", sut.state.Value);

            sut.NormalValue = 1.0f;
            Assert.AreEqual("p", sut.state.Value);
        }

        [Test]
        public void TestStateForMid()
        {
            var sut = new SegmentState<string>("i");

            Assert.AreEqual("i", sut.state.Value);

            sut.segments.Add(new SegmentState<string>.Segment(1.0f, "p"));
            sut.segments.Add(new SegmentState<string>.Segment(1.0f, "z"));
            Assert.AreEqual(2.0f, sut.Size);

            sut.NormalValue = 0;
            Assert.AreEqual("p", sut.state.Value);

            sut.NormalValue = 1.0f / 2.0f;
            Assert.AreEqual("p", sut.state.Value);

            sut.NormalValue = 1.5f / 2.0f;
            Assert.AreEqual("z", sut.state.Value);

            sut.NormalValue = 2.0f / 2.0f;
            Assert.AreEqual("z", sut.state.Value);
        }
    }
}
