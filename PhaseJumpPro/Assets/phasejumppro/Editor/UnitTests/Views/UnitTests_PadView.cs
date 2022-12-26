using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_PadView
    {
        protected class TestPadView : PJ.PadView {
            public void TestApplyLayout(Bounds2D parentBounds) {
                _ApplyLayout(parentBounds.size);
            }
        }

        [Test]
        public void TestIntrinsicWidth_ChildHasIntrinsicWidth()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestPadView>();
            sut.padLeading = 3.0f;
            sut.padTrailing = 5.0f;

            var childObject = new GameObject();
            var childSut = childObject.AddComponent<View2D>();
            childSut.IntrinsicWidth = new(10.0f);

            childObject.transform.parent = gameObject.transform;

            Assert.AreEqual(18.0f, sut.DefaultIntrinsicWidth);
        }
        
        [Test]
        public void TestIntrinsicHeight_ChildHasIntrinsicHeight()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestPadView>();
            sut.padTop = 3.0f;
            sut.padBottom = 5.0f;

            var childObject = new GameObject();
            var childSut = childObject.AddComponent<View2D>();
            childSut.IntrinsicHeight = new(10.0f);

            childObject.transform.parent = gameObject.transform;

            Assert.AreEqual(18.0f, sut.DefaultIntrinsicHeight);
        }

        [Test]
        public void TestIntrinsicWidth_ChildHasNoIntrinsicWidth()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestPadView>();

            var childObject = new GameObject();
            var childSut = childObject.AddComponent<View2D>();

            childObject.transform.parent = gameObject.transform;

            Assert.AreEqual(null, sut.IntrinsicWidth);
        }

        [Test]
        public void TestIntrinsicHeight_ChildHasNoIntrinsicHeight()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestPadView>();

            var childObject = new GameObject();
            var childSut = childObject.AddComponent<View2D>();

            childObject.transform.parent = gameObject.transform;

            Assert.AreEqual(null, sut.IntrinsicHeight);
        }

        [Test]
        public void TestApplyLayout_ChildHasNoIntrinsicSize()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestPadView>();
            sut.padLeading = 3.0f;
            sut.padTrailing = 6.0f;
            sut.padTop = 2.0f;
            sut.padBottom = 4.0f;

            var childObject = new GameObject();
            var childSut = childObject.AddComponent<View2D>();

            childObject.transform.parent = gameObject.transform;

            sut.TestApplyLayout(new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(new Bounds2D(new Vector2(3.0f, 2.0f), new Vector2(1.0f, 4.0f)), childSut.Frame);
        }

        [Test]
        public void TestApplyLayout_ChildHasIntrinsicSize()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestPadView>();
            sut.padLeading = 3.0f;
            sut.padTrailing = 2.0f;
            sut.padTop = 2.0f;
            sut.padBottom = 4.0f;

            var childObject = new GameObject();
            var childSut = childObject.AddComponent<View2D>();
            childSut.IntrinsicWidth = new(1.0f);
            childSut.IntrinsicHeight = new(3.0f);

            childObject.transform.parent = gameObject.transform;

            sut.TestApplyLayout(new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(new Bounds2D(new Vector2(3.0f, 2.0f), new Vector2(1.0f, 3.0f)), childSut.Frame);
        }
    }
}
