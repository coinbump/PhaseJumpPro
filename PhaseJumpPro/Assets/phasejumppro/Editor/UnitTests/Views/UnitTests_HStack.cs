using System;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_HStack
    {
        protected class TestHStack : PJ.HStack
        {
            public void TestApplyLayout(Bounds2D parentBounds)
            {
                _ApplyLayout(parentBounds.size);
            }
        }

        [Test]
        public void TestWidths_NoChildrenWithIntrinsicWidth()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestHStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;

            sut.TestApplyLayout(new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(5.0f, childView1.Frame.size.x);
            Assert.AreEqual(5.0f, childView2.Frame.size.x);
            Assert.AreEqual(10.0f, childView1.Frame.size.y);
            Assert.AreEqual(10.0f, childView2.Frame.size.y);
        }

        [Test]
        public void TestWidths_MixedChildrenWithIntrinsicWidth()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestHStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;
            childView1.IntrinsicWidth = new(3.0f);

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;

            sut.TestApplyLayout(new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(3.0f, childView1.Frame.size.x);
            Assert.AreEqual(7.0f, childView2.Frame.size.x);
            Assert.AreEqual(10.0f, childView1.Frame.size.y);
            Assert.AreEqual(10.0f, childView2.Frame.size.y);
        }

        [Test]
        public void TestWidths_AllChildrenWithIntrinsicWidth()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestHStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;
            childView1.IntrinsicWidth = new(3.0f);

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;
            childView2.IntrinsicWidth = new(3.0f);

            sut.TestApplyLayout(new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(3.0f, childView1.Frame.size.x);
            Assert.AreEqual(3.0f, childView2.Frame.size.x);
            Assert.AreEqual(10.0f, childView1.Frame.size.y);
            Assert.AreEqual(10.0f, childView2.Frame.size.y);
        }

        [Test]
        public void TestPreferredHeight_AllChildrenWithIntrinsicHeight()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestHStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;
            childView1.IntrinsicHeight = new(3.0f);

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;
            childView2.IntrinsicHeight = new(7.0f);

            sut.TestApplyLayout(new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(3.0f, childView1.Frame.size.y);
            Assert.AreEqual(7.0f, childView2.Frame.size.y);
            Assert.AreEqual(7.0f, sut.PreferredHeight(Vector2.zero));
        }

        [Test]
        public void TestPreferredHeight_MixedChildrenWithIntrinsicHeight()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestHStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;
            childView1.IntrinsicHeight = new(7.0f);

            sut.TestApplyLayout(new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(7.0f, childView1.Frame.size.y);
            Assert.AreEqual(7.0f, childView2.Frame.size.y);
            Assert.AreEqual(7.0f, sut.PreferredHeight(Vector2.zero));
        }

        [Test]
        public void TestSpacing_NoChildrenWithIntrinsicWidth()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestHStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;

            sut.spacing = 1.0f;
            sut.TestApplyLayout(new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(4.5f, childView1.Frame.size.x);
            Assert.AreEqual(4.5f, childView2.Frame.size.x);
            Assert.AreEqual(5.5f, childView2.Frame.origin.x);
        }
    }
}
