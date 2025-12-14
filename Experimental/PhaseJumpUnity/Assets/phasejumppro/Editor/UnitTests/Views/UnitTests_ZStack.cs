using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_ZStack
    {
        protected class TestZStack : PJ.ZStack
        {
            public void TestApplyLayout(Rect parentBounds)
            {
                _ApplyLayout(parentBounds.size);
            }
        }

        [Test]
        public void TestSize_NoChildrenWithIntrinsicSize()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestZStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;

            sut.TestApplyLayout(new Rect(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(10.0f, childView1.Frame.size.x);
            Assert.AreEqual(10.0f, childView2.Frame.size.x);
            Assert.AreEqual(10.0f, childView1.Frame.size.y);
            Assert.AreEqual(10.0f, childView2.Frame.size.y);
        }

        [Test]
        public void TestIntrinsicSize_NoChildrenWithIntrinsicSize()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestZStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;

            Assert.AreEqual(null, sut.IntrinsicWidth);
            Assert.AreEqual(null, sut.IntrinsicHeight);
        }

        [Test]
        public void TestPreferredSize_ChildrenWithIntrinsicSize()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestZStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;
            childView1.IntrinsicWidth = new(3.0f);
            childView1.isFixedWidth = true;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;
            childView2.IntrinsicHeight = new(7.0f);
            childView2.isFixedHeight = true;

            Assert.AreEqual(3.0f, sut.PreferredWidthLayoutQuery(new(0, 0), new() { LayoutQueryOption.WithConstraints }));
            Assert.AreEqual(7.0f, sut.PreferredHeightWithConstraints(Vector2.zero).value);
        }
    }
}
