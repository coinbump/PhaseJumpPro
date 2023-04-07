using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_CombinedStacks
    {
        protected class TestHStack : PJ.HStack
        {
            public void TestApplyLayout(Bounds2D layoutBounds)
            {
                Frame = layoutBounds;
                _ApplyLayout(layoutBounds.size);
                _PostApplyLayout();
            }
        }

        protected class TestVStack : PJ.VStack
        {
            public void TestApplyLayout(Bounds2D layoutBounds)
            {
                _ApplyLayout(layoutBounds.size);
            }
        }

        [Test]
        public void Test_NoChildrenWithIntrinsicWidth()
        {

            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestHStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<TestVStack>();
            child1.transform.parent = gameObject.transform;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;

            var vChild1 = new GameObject();
            var vChildView1 = vChild1.AddComponent<View2D>();
            vChild1.transform.parent = child1.transform;

            var vChild2 = new GameObject();
            var vChildView2 = vChild2.AddComponent<View2D>();
            vChild2.transform.parent = child1.transform;

            sut.TestApplyLayout(new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(5.0f, childView1.Frame.size.x);
            Assert.AreEqual(5.0f, childView2.Frame.size.x);
            Assert.AreEqual(10.0f, childView1.Frame.size.y);
            Assert.AreEqual(10.0f, childView2.Frame.size.y);

            Assert.AreEqual(5.0f, vChildView1.Frame.size.x);
            Assert.AreEqual(5.0f, vChildView2.Frame.size.x);
            Assert.AreEqual(5.0f, vChildView1.Frame.size.y);
            Assert.AreEqual(5.0f, vChildView2.Frame.size.y);
        }

        [Test]
        public void Test_ChildrenWithIntrinsicWidth()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestHStack>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<TestVStack>();
            child1.transform.parent = gameObject.transform;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;

            var vChild1 = new GameObject();
            var vChildView1 = vChild1.AddComponent<View2D>();
            vChildView1.IntrinsicWidth = new(3.0f);
            vChild1.transform.parent = child1.transform;

            var vChild2 = new GameObject();
            var vChildView2 = vChild2.AddComponent<View2D>();
            vChildView2.IntrinsicWidth = new(7.0f);
            vChild2.transform.parent = child1.transform;

            sut.TestApplyLayout(new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f)));

            Assert.AreEqual(5.0f, childView1.Frame.size.x);
            Assert.AreEqual(5.0f, childView2.Frame.size.x);
            Assert.AreEqual(10.0f, childView1.Frame.size.y);
            Assert.AreEqual(10.0f, childView2.Frame.size.y);

            Assert.AreEqual(3.0f, vChildView1.Frame.size.x);
            Assert.AreEqual(7.0f, vChildView2.Frame.size.x);
            Assert.AreEqual(5.0f, vChildView1.Frame.size.y);
            Assert.AreEqual(5.0f, vChildView2.Frame.size.y);
        }
    }
}
