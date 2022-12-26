using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_View2D
    {
        [Test]
        public void TestWorldSize()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Bounds2D(Vector2.zero, new Vector2(2.0f, 2.0f));
            
            Assert.AreEqual(new Vector2(2.0f, 2.0f), sut.WorldSize);
        }

        [Test]
        public void Test_LocalToViewPosition()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f));

            Assert.AreEqual(new Vector3(5.0f, 5.0f, 0), sut.LocalToViewPosition(new Vector3(0, 0, 0)));
            Assert.AreEqual(new Vector3(0.0f, 0.0f, 0), sut.LocalToViewPosition(new Vector3(-5.0f, 5.0f, 0)));
        }

        [Test]
        public void Test_TopLeftWorldPosition()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f));
            
            Assert.AreEqual(new Vector3(-5.0f, 5.0f, 0), sut.TopLeftWorldPosition(new Vector3(0, 0, 0)));
        }

        [Test]
        public void Test_IsViewPositionInside()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f));

            Assert.AreEqual(true, sut.IsViewPositionInside(new Vector3(0, 0, 0)));
            Assert.AreEqual(true, sut.IsViewPositionInside(new Vector3(0.5f, 0.5f, 0)));
            Assert.AreEqual(false, sut.IsViewPositionInside(new Vector3(-0.5f, 0.5f, 0)));
            Assert.AreEqual(false, sut.IsViewPositionInside(new Vector3(0.5f, -0.5f, 0)));
            Assert.AreEqual(false, sut.IsViewPositionInside(new Vector3(10.5f, 10.5f, 0)));
        }

        [Test]
        public void Test_TopLeftLocalPositionIn()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Bounds2D(Vector2.zero, new Vector2(2.0f, 2.0f));

            var parentFrame = new Bounds2D(Vector2.zero, new Vector2(4.0f, 4.0f));

            Assert.AreEqual(new Vector3(-2.0f, 2.0f, 0), sut.TopLeftLocalPositionIn(parentFrame, new Vector3(0, 0, 0)));

            gameObject = new GameObject();
            sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Bounds2D(new Vector2(1.0f, 1.0f), new Vector2(2.0f, 2.0f));
            Assert.AreEqual(new Vector3(-1.0f, 1.0f, 0), sut.TopLeftLocalPositionIn(parentFrame, new Vector3(0, 0, 0)));
        }

        [Test]
        public void Test_LocalPositionIn()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Bounds2D(Vector2.zero, new Vector2(2.0f, 2.0f));
            var parentFrame = new Bounds2D(Vector2.zero, new Vector2(4.0f, 4.0f));

            Assert.AreEqual(new Vector3(-1.0f, 1.0f, 0), sut.LocalPositionIn(parentFrame, new Vector3(0, 0, 0)));

            gameObject = new GameObject();
            sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Bounds2D(new Vector2(1.0f, 1.0f), new Vector2(2.0f, 2.0f));
            Assert.AreEqual(new Vector3(0, 0, 0), sut.LocalPositionIn(parentFrame, new Vector3(0, 0, 0)));
        }

        [Test]
        public void TestParentView()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
          
            var child = new GameObject();
            var childView = child.AddComponent<View2D>();
            child.transform.parent = gameObject.transform;

            Assert.AreEqual(sut, childView.ParentView());
        }

        [Test]
        public void TestRootView()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();

            var child = new GameObject();
            var childView = child.AddComponent<View2D>();
            child.transform.parent = gameObject.transform;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = child.transform;

            Assert.AreEqual(sut, childView.RootView());
            Assert.AreEqual(sut, childView2.RootView());
        }

        [Test]
        public void TestChildViews()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();

            var child = new GameObject();
            var childView = child.AddComponent<View2D>();
            child.transform.parent = gameObject.transform;
            
            Assert.AreEqual(1, sut.ChildViews().Count);
            Assert.AreEqual(childView, sut.ChildViews()[0]);
        }

        [Test]
        public void Test_PreferredSize_NoIntrinsicSize()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();

            Assert.AreEqual(10.0f, sut.PreferredWidth(10.0f));
            Assert.AreEqual(10.0f, sut.PreferredHeight(10.0f));
        }

        [Test]
        public void Test_PreferredSize_HasIntrinsicSize()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.IntrinsicWidth = new(3.0f);
            sut.IntrinsicHeight = new(7.0f);

            Assert.AreEqual(3.0f, sut.PreferredWidth(10.0f));
            Assert.AreEqual(7.0f, sut.PreferredHeight(10.0f));
        }

        [Test]
        public void Test_PreferredSize_HasMinMaxSize()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.tags["width.min"] = 3.0f;
            sut.tags["width.max"] = 4.0f;
            sut.tags["height.min"] = 2.0f;
            sut.tags["height.max"] = 7.0f;

            Assert.AreEqual(4.0f, sut.PreferredWidth(10.0f));
            Assert.AreEqual(7.0f, sut.PreferredHeight(10.0f));

            Assert.AreEqual(3.0f, sut.PreferredWidth(3.0f));
            Assert.AreEqual(3.0f, sut.PreferredHeight(3.0f));
        }
    }
}
