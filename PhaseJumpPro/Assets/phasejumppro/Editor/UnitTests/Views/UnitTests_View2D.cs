using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_View2D
    {
        private class TestView2D : View2D
        {
            public int frameChangeCount = 0;

            protected override void OnFrameChange()
            {
                base.OnFrameChange();
                frameChangeCount++;
            }

            public override Rect ParentBounds()
            {
                return new(new(0, 0), new(10, 10));
            }
        }

        [Test]
        public void TestWorldSize()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Rect(Vector2.zero, new Vector2(2.0f, 2.0f));

            Assert.AreEqual(new Vector2(2.0f, 2.0f), sut.WorldSize);
        }

        [Test]
        public void Test_FrameChange()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestView2D>();

            var initialState = sut.frameChangeCount;

            // This won't work because OnValidate is called while running unit tests
            // Assert.AreEqual(0, sut.frameChangeCount);

            sut.Frame = new Rect(Vector2.zero, new Vector2(2.0f, 2.0f));

            Assert.AreEqual(initialState + 1, sut.frameChangeCount);
            sut.Frame = new Rect(Vector2.zero, new Vector2(2.0f, 2.0f));
            Assert.AreEqual(initialState + 1, sut.frameChangeCount);
        }

        [Test]
        public void Test_LocalToViewPosition()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Rect(Vector2.zero, new Vector2(10.0f, 10.0f));

            Assert.AreEqual(new Vector3(5.0f, 5.0f, 0), sut.LocalToViewPosition(new Vector3(0, 0, 0)));
            Assert.AreEqual(new Vector3(0.0f, 0.0f, 0), sut.LocalToViewPosition(new Vector3(-5.0f, 5.0f, 0)));
        }

        [Test]
        public void Test_TopLeftWorldPosition()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Rect(Vector2.zero, new Vector2(10.0f, 10.0f));

            Assert.AreEqual(new Vector3(-5.0f, 5.0f, 0), sut.TopLeftWorldPosition(new Vector3(0, 0, 0)));
        }

        [Test]
        public void Test_ViewToWorldPosition()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Rect(Vector2.zero, new Vector2(10.0f, 10.0f));

            Assert.AreEqual(new Vector3(-4.0f, 4.0f, 0), sut.ViewToWorldPosition(new Vector2(1, 1)));
            Assert.AreEqual(new Vector3(5.0f, -5.0f, 0), sut.ViewToWorldPosition(new Vector2(10, 10)));
        }

        [Test]
        public void Test_WorldToViewPosition()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Rect(Vector2.zero, new Vector2(10.0f, 10.0f));

            Assert.AreEqual(new Vector2(1, 1), sut.WorldToViewPosition(new Vector3(-4.0f, 4.0f, 0)));
            Assert.AreEqual(new Vector2(10, 10), sut.WorldToViewPosition(new Vector3(5.0f, -5.0f, 0)));
        }

        [Test]
        public void Test_IsViewPositionInside()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Rect(Vector2.zero, new Vector2(10.0f, 10.0f));

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
            sut.Frame = new Rect(Vector2.zero, new Vector2(2.0f, 2.0f));

            var parentFrame = new Rect(Vector2.zero, new Vector2(4.0f, 4.0f));

            Assert.AreEqual(new Vector3(-2.0f, 2.0f, 0), sut.TopLeftLocalPositionIn(parentFrame, new Vector3(0, 0, 0)));

            gameObject = new GameObject();
            sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Rect(new Vector2(1.0f, 1.0f), new Vector2(2.0f, 2.0f));
            Assert.AreEqual(new Vector3(-1.0f, 1.0f, 0), sut.TopLeftLocalPositionIn(parentFrame, new Vector3(0, 0, 0)));
        }

        [Test]
        public void Test_LocalPositionIn()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Rect(Vector2.zero, new Vector2(2.0f, 2.0f));
            var parentFrame = new Rect(Vector2.zero, new Vector2(4.0f, 4.0f));

            Assert.AreEqual(new Vector3(-1.0f, 1.0f, 0), sut.LocalPositionIn(parentFrame, new Vector3(0, 0, 0)));

            gameObject = new GameObject();
            sut = gameObject.AddComponent<View2D>();
            sut.Frame = new Rect(new Vector2(1.0f, 1.0f), new Vector2(2.0f, 2.0f));
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

            Assert.AreEqual(sut, childView.ParentView);
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

            Assert.AreEqual(sut, childView.RootView);
            Assert.AreEqual(sut, childView2.RootView);
        }

        [Test]
        public void TestChildViews()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;

            Assert.AreEqual(2, sut.ChildViews().Count);
            Assert.AreEqual(childView1, sut.ChildViews()[0]);
            Assert.AreEqual(childView2, sut.ChildViews()[1]);
        }

        [Test]
        public void TestGraphViews()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;

            var child3 = new GameObject();
            var childView3 = child3.AddComponent<View2D>();
            child3.transform.parent = child1.transform;

            var graphViews = sut.GraphViews();

            Assert.AreEqual(4, graphViews.Count);
            Assert.AreEqual(sut, graphViews[0]);
            Assert.AreEqual(childView1, graphViews[1]);
            Assert.AreEqual(childView3, graphViews[2]);
            Assert.AreEqual(childView2, graphViews[3]);
        }

        [Test]
        public void Test_NotFixedWidth_ChildrenNoIntrinsicWidth_PreferredWidthWithoutConstraints()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.isFixedWidth = false;

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;

            Assert.AreEqual(null, sut.PreferredWidthWithoutConstraints(new Vector2(10, 10)));
        }

        [Test]
        public void Test_IsFixedWidth_ChildrenNoIntrinsicWidth_PreferredWidthWithoutConstraints()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.isFixedWidth = true;

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;

            Assert.AreEqual(null, sut.PreferredWidthWithoutConstraints(new Vector2(10, 10)));
        }

        [Test]
        public void Test_IsFixedWidth_ChildrenWithIntrinsicWidth_PreferredWidthWithoutConstraints()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.isFixedWidth = true;

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;
            childView1.IntrinsicWidth = new(5.0f);
            childView1.isFixedWidth = true;

            Assert.AreEqual(5.0f, sut.PreferredWidthWithoutConstraints(new Vector2(10, 10)).value);
        }

        [Test]
        public void Test_WidthWithConstraints()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();

            sut.MaxWidth = new(9.0f);
            Assert.AreEqual(9.0f, sut.WidthWithConstraints(10.0f));
        }

        [Test]
        public void Test_PreferredWidthWithConstraints()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.IsFixedWidth = true;

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;
            childView1.IntrinsicWidth = new(10.0f);
            childView1.isFixedWidth = true;

            sut.MaxWidth = new(9.0f);
            Assert.AreEqual(9.0f, sut.PreferredWidthWithConstraints(new Vector2(10, 10)).value);
        }

        [Test]
        public void Test_PreferredWidthLayoutQuery()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();

            sut.MaxWidth = new(9.0f);
            Assert.AreEqual(9.0f, sut.PreferredWidthLayoutQuery(new Vector2(10, 10), new() { LayoutQueryOption.WithConstraints }));
        }

        [Test]
        public void Test_HeightWithConstraints()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();

            sut.MaxHeight = new(9.0f);
            Assert.AreEqual(9.0f, sut.HeightWithConstraints(10.0f));
        }

        [Test]
        public void Test_PreferredHeightWithConstraints()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.IsFixedHeight = true;

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;
            childView1.IntrinsicHeight = new(10.0f);
            childView1.isFixedHeight = true;

            sut.MaxHeight = new(9.0f);
            Assert.AreEqual(9.0f, sut.PreferredHeightWithConstraints(new Vector2(10, 10)).value);
        }

        [Test]
        public void Test_PreferredHeightLayoutQuery()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();

            sut.MaxHeight = new(9.0f);
            Assert.AreEqual(9.0f, sut.PreferredHeightLayoutQuery(new Vector2(10, 10), new() { LayoutQueryOption.WithConstraints }));
        }

        [Test]
        public void TestFirstChildView()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;

            Assert.AreEqual(childView1, sut.FirstChildView);
        }

        [Test]
        public void Test_PreferredSize_NoIntrinsicSize()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();

            Assert.AreEqual(10.0f, sut.PreferredWidthLayoutQuery(new(10.0f, 0), new() { LayoutQueryOption.WithConstraints }));
            Assert.AreEqual(10.0f, sut.PreferredHeightLayoutQuery(new Vector2(10.0f, 10.0f), new() { LayoutQueryOption.WithConstraints }));
        }

        [Test]
        public void Test_PreferredSize_HasIntrinsicSize()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<View2D>();
            sut.IntrinsicWidth = new(3.0f);
            sut.IntrinsicHeight = new(7.0f);
            sut.isFixedWidth = true;
            sut.isFixedHeight = true;

            Assert.AreEqual(3.0f, sut.PreferredWidthLayoutQuery(new(10.0f, 0), new() { LayoutQueryOption.WithConstraints }));
            Assert.AreEqual(7.0f, sut.PreferredHeightWithConstraints(new Vector2(3.0f, 10.0f)).value);
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

            Assert.AreEqual(4.0f, sut.PreferredWidthLayoutQuery(new(10.0f, 0), new() { LayoutQueryOption.WithConstraints }));
            Assert.AreEqual(7.0f, sut.PreferredHeightLayoutQuery(new Vector2(4.0f, 10.0f), new() { LayoutQueryOption.WithConstraints }));

            Assert.AreEqual(3.0f, sut.PreferredWidthLayoutQuery(new(3.0f, 0), new() { LayoutQueryOption.WithConstraints }));
            Assert.AreEqual(3.0f, sut.PreferredHeightLayoutQuery(new Vector2(3.0f, 3.0f), new() { LayoutQueryOption.WithConstraints }));
        }

        [Test]
        public void TestApplyLayout()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestView2D>();

            var child1 = new GameObject();
            var childView1 = child1.AddComponent<View2D>();
            child1.transform.parent = gameObject.transform;
            childView1.FixedWidth = new(3.0f);
            childView1.FixedHeight = new(5.0f);

            var child2 = new GameObject();
            var childView2 = child2.AddComponent<View2D>();
            child2.transform.parent = gameObject.transform;
            childView2.FixedWidth = new(2.0f);
            childView2.FixedHeight = new(6.0f);

            sut.ApplyLayout(true);

            Assert.AreEqual(new Rect(new(5.0f - 1.5f, 5.0f - 2.5f), new(3.0f, 5.0f)), childView1.Frame);
            Assert.AreEqual(new Rect(new(5.0f - 1.0f, 5.0f - 3.0f), new(2.0f, 6.0f)), childView2.Frame);
        }
    }
}
