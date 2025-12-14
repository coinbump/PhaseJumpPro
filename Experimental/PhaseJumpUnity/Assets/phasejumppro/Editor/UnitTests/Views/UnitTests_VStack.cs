using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_VStack
    {
        protected class TestVStack : PJ.VStack
        {
            public void TestApplyLayout(Rect parentBounds)
            {
                _ApplyLayout(parentBounds.size);
            }
        }

        [Test]
        public void TestHeights_NoChildrenWithIntrinsicHeight()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestVStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;

            sut.TestApplyLayout(new Rect(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(5.0f, childView1.Frame.size.y);
            Assert.AreEqual(5.0f, childView2.Frame.size.y);
            Assert.AreEqual(10.0f, childView1.Frame.size.x);
            Assert.AreEqual(10.0f, childView2.Frame.size.x);
        }

        [Test]
        public void TestHeights_MixedChildrenWithIntrinsicHeight()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestVStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;
            childView1.IntrinsicHeight = new(3.0f);
            childView1.isFixedHeight = true;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;

            sut.TestApplyLayout(new Rect(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(3.0f, childView1.Frame.size.y);
            Assert.AreEqual(7.0f, childView2.Frame.size.y);
            Assert.AreEqual(10.0f, childView1.Frame.size.x);
            Assert.AreEqual(10.0f, childView2.Frame.size.x);
        }

        [Test]
        public void TestHeights_AllChildrenWithIntrinsicHeight()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestVStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;
            childView1.IntrinsicHeight = new(3.0f);
            childView1.isFixedHeight = true;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;
            childView2.IntrinsicHeight = new(3.0f);
            childView2.isFixedHeight = true;

            sut.TestApplyLayout(new Rect(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(3.0f, childView1.Frame.size.y);
            Assert.AreEqual(3.0f, childView2.Frame.size.y);
            Assert.AreEqual(10.0f, childView1.Frame.size.x);
            Assert.AreEqual(10.0f, childView2.Frame.size.x);
        }

        [Test]
        public void TestPreferredWidth_AllChildrenWithIntrinsicWidth()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestVStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;
            childView1.IntrinsicWidth = new(3.0f);
            childView1.isFixedWidth = true;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;
            childView2.IntrinsicWidth = new(7.0f);
            childView2.isFixedWidth = true;

            sut.TestApplyLayout(new Rect(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(3.0f, childView1.Frame.size.x);
            Assert.AreEqual(7.0f, childView2.Frame.size.x);
            Assert.AreEqual(7.0f, sut.PreferredWidthLayoutQuery(new(10.0f, 0), new() { LayoutQueryOption.WithConstraints }));
        }

        [Test]
        public void TestPreferredWidth_MixedChildrenWithIntrinsicWidth()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestVStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;
            childView1.IntrinsicWidth = new(7.0f);
            childView1.isFixedWidth = true;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;

            sut.TestApplyLayout(new Rect(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(7.0f, childView1.Frame.size.x);
            Assert.AreEqual(7.0f, childView2.Frame.size.x);
            Assert.AreEqual(7.0f, sut.PreferredWidthLayoutQuery(new(10.0f, 0), new() { LayoutQueryOption.WithConstraints }));
        }

        [Test]
        public void TestSpacing_NoChildrenWithIntrinsicHeight()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestVStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;

            sut.spacing = 2.0f;
            sut.TestApplyLayout(new Rect(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(4.0f, childView1.Frame.size.y);
            Assert.AreEqual(4.0f, childView2.Frame.size.y);
            Assert.AreEqual(6.0f, childView2.Frame.origin.y);
        }
    }
}
