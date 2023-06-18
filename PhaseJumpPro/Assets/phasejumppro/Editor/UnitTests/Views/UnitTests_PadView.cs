using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_PadView
    {
        protected class TestPadView : PJ.PadView
        {
            public void TestApplyLayout(Rect parentBounds)
            {
                _ApplyLayout(parentBounds.size);
            }
        }

        [Test]
        public void TestPreferredWidth_ChildHasIntrinsicWidth()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestPadView>();
            sut.padLeading = 3.0f;
            sut.padTrailing = 5.0f;

            var childObject = new GameObject();
            var childSut = childObject.AddComponent<View2D>();
            childSut.IntrinsicWidth = new(10.0f);
            childSut.isFixedWidth = true;

            childObject.transform.parent = gameObject.transform;

            Assert.AreEqual(18.0f, sut.PreferredWidthLayoutQuery(new(0, 0), new() { LayoutQueryOption.WithConstraints }));
        }

        [Test]
        public void TestPreferredHeight_ChildHasIntrinsicHeight()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestPadView>();
            sut.padTop = 3.0f;
            sut.padBottom = 5.0f;

            var childObject = new GameObject();
            var childSut = childObject.AddComponent<View2D>();
            childSut.IntrinsicHeight = new(10.0f);
            childSut.isFixedHeight = true;

            childObject.transform.parent = gameObject.transform;

            Assert.AreEqual(18.0f, sut.PreferredHeightWithConstraints(Vector2.zero).value);
        }

        [Test]
        public void TestPreferredWidth_ChildHasNoIntrinsicWidth()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestPadView>();

            var childObject = new GameObject();
            var childSut = childObject.AddComponent<View2D>();
            childSut.IntrinsicWidth = new(8.0f);
            childSut.isFixedWidth = true;

            childObject.transform.parent = gameObject.transform;

            Assert.AreEqual(8.0f, sut.PreferredWidthLayoutQuery(new Vector2(10, 10), new() { LayoutQueryOption.WithConstraints }));
        }

        [Test]
        public void TestPreferredHeight_ChildHasNoIntrinsicHeight()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestPadView>();

            var childObject = new GameObject();
            var childSut = childObject.AddComponent<View2D>();
            childSut.IntrinsicHeight = new(8.0f);
            childSut.isFixedHeight = true;

            childObject.transform.parent = gameObject.transform;

            Assert.AreEqual(8.0f, sut.PreferredHeightLayoutQuery(new Vector2(10, 10), new() { LayoutQueryOption.WithConstraints }));
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

            sut.TestApplyLayout(new Rect(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(new Rect(new Vector2(3.0f, 2.0f), new Vector2(0.0f, 0.0f)), childSut.Frame);
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
            childSut.isFixedWidth = true;
            childSut.isFixedHeight = true;

            childObject.transform.parent = gameObject.transform;

            sut.TestApplyLayout(new Rect(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(new Rect(new Vector2(3.0f, 2.0f), new Vector2(1.0f, 3.0f)), childSut.Frame);
        }
    }
}
