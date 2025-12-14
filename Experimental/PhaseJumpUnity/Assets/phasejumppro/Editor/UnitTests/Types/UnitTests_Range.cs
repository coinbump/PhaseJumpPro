using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_Range
    {
        [Test]
        public void Test_InclusiveForward()
        {
            var sut = Range<int>.InclusiveRange(3, 10);

            Assert.IsFalse(sut.IsInside(2));
            Assert.IsTrue(sut.IsInside(3));
            Assert.IsTrue(sut.IsInside(4));
            Assert.IsTrue(sut.IsInside(9));
            Assert.IsTrue(sut.IsInside(10));
            Assert.IsFalse(sut.IsInside(11));
        }

        [Test]
        public void Test_InclusiveReverse()
        {
            var sut = Range<int>.InclusiveRange(10, 3);

            Assert.IsFalse(sut.IsInside(2));
            Assert.IsTrue(sut.IsInside(3));
            Assert.IsTrue(sut.IsInside(4));
            Assert.IsTrue(sut.IsInside(9));
            Assert.IsTrue(sut.IsInside(10));
            Assert.IsFalse(sut.IsInside(11));
        }

        [Test]
        public void Test_InsideForward()
        {
            var sut = Range<int>.InsideRange(3, 10);

            Assert.IsFalse(sut.IsInside(2));
            Assert.IsFalse(sut.IsInside(3));
            Assert.IsTrue(sut.IsInside(4));
            Assert.IsTrue(sut.IsInside(9));
            Assert.IsFalse(sut.IsInside(10));
            Assert.IsFalse(sut.IsInside(11));
        }

        [Test]
        public void Test_InsideReverse()
        {
            var sut = Range<int>.InsideRange(10, 3);

            Assert.IsFalse(sut.IsInside(2));
            Assert.IsFalse(sut.IsInside(3));
            Assert.IsTrue(sut.IsInside(4));
            Assert.IsTrue(sut.IsInside(9));
            Assert.IsFalse(sut.IsInside(10));
            Assert.IsFalse(sut.IsInside(11));
        }

        [Test]
        public void Test_OpenForward()
        {
            var sut = new Range<int>(3, 10, Range<int>.Evaluate.Inclusive, Range<int>.Evaluate.Open);

            Assert.IsFalse(sut.IsInside(2));
            Assert.IsTrue(sut.IsInside(3));
            Assert.IsTrue(sut.IsInside(4));
            Assert.IsTrue(sut.IsInside(10));
            Assert.IsTrue(sut.IsInside(11));
        }

        [Test]
        public void Test_OpenReverse()
        {
            var sut = new Range<int>(10, 3, Range<int>.Evaluate.Inclusive, Range<int>.Evaluate.Open);

            Assert.IsTrue(sut.IsInside(2));
            Assert.IsTrue(sut.IsInside(3));
            Assert.IsTrue(sut.IsInside(4));
            Assert.IsTrue(sut.IsInside(10));
            Assert.IsFalse(sut.IsInside(11));
        }
    }
}
