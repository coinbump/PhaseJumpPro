using System;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_VGridView
    {
        protected class TestVGridView : PJ.VGridView {
            public void TestApplyLayout(Bounds2D parentBounds) {
                _ApplyLayout(parentBounds.size);
            }
        }

        protected View2D AddChildViewTo(GameObject parent)
        {
            var child = new GameObject();
            var childView = child.AddComponent<View2D>();
            child.transform.parent = parent.transform;

            return childView;
        }

        [Test]
        public void Test_Flexible_SingleRow_NoChildrenWithIntrinsicWidth()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestVGridView>();
            sut.rowSpacing = 0.0f;
            sut.columns.Add(new(GridViewItem.SizeType.Flexible, 0, Alignment.center, 2));
            sut.columns.Add(new(GridViewItem.SizeType.Flexible, 0, Alignment.center, 2));

            var childView1 = AddChildViewTo(gameObject);
            var childView2 = AddChildViewTo(gameObject);

            sut.TestApplyLayout(new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f)));
            
            Assert.AreEqual(4.0f, childView1.Frame.size.x);
            Assert.AreEqual(4.0f, childView2.Frame.size.x);

            Assert.AreEqual(0.0f, childView1.Frame.origin.x);
            Assert.AreEqual(6.0f, childView2.Frame.origin.x);

            Assert.AreEqual(10.0f, childView1.Frame.size.y);
            Assert.AreEqual(10.0f, childView2.Frame.size.y);
        }

        [Test]
        public void Test_Flexible_MultiRow_NoChildrenWithIntrinsicWidth()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestVGridView>();
            sut.rowSpacing = 3.0f;
            sut.columns.Add(new(GridViewItem.SizeType.Flexible, 0, Alignment.center, 2));
            sut.columns.Add(new(GridViewItem.SizeType.Flexible, 0, Alignment.center, 2));

            var childView1 = AddChildViewTo(gameObject);
            var childView2 = AddChildViewTo(gameObject);
            var childView3 = AddChildViewTo(gameObject);

            sut.TestApplyLayout(new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f)));
            
            Assert.AreEqual(4.0f, childView1.Frame.size.x);
            Assert.AreEqual(4.0f, childView2.Frame.size.x);
            Assert.AreEqual(4.0f, childView3.Frame.size.x);

            Assert.AreEqual(0.0f, childView1.Frame.origin.x);
            Assert.AreEqual(6.0f, childView2.Frame.origin.x);
            Assert.AreEqual(0.0f, childView3.Frame.origin.x);

            Assert.AreEqual(3.5f, childView1.Frame.size.y);
            Assert.AreEqual(3.5f, childView2.Frame.size.y);
            Assert.AreEqual(3.5f, childView3.Frame.size.y);

            Assert.AreEqual(0.0f, childView1.Frame.origin.y);
            Assert.AreEqual(0.0f, childView2.Frame.origin.y);
            Assert.AreEqual(6.5f, childView3.Frame.origin.y);
        }

        [Test]
        public void Test_Fixed_SingleRow_NoChildrenWithIntrinsicWidth_CenterAlign()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestVGridView>();
            sut.rowSpacing = 0.0f;
            sut.columns.Add(new(GridViewItem.SizeType.Fixed, 3.0f, Alignment.center, 2));
            sut.columns.Add(new(GridViewItem.SizeType.Fixed, 4.0f, Alignment.center, 2));

            var childView1 = AddChildViewTo(gameObject);
            var childView2 = AddChildViewTo(gameObject);

            sut.TestApplyLayout(new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f)));
            
            Assert.AreEqual(3.0f, childView1.Frame.size.x);
            Assert.AreEqual(4.0f, childView2.Frame.size.x);

            Assert.AreEqual(1.5f, childView1.Frame.origin.x);
            Assert.AreEqual(6.5f, childView2.Frame.origin.x);

            Assert.AreEqual(10.0f, childView1.Frame.size.y);
            Assert.AreEqual(10.0f, childView2.Frame.size.y);
        }

        [Test]
        public void Test_Fixed_SingleRow_NoChildrenWithIntrinsicWidth_LeadingAlign()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<TestVGridView>();
            sut.rowSpacing = 0.0f;
            sut.alignment = HorizontalAlignment.leading;
            sut.columns.Add(new(GridViewItem.SizeType.Fixed, 3.0f, Alignment.center, 2));
            sut.columns.Add(new(GridViewItem.SizeType.Fixed, 4.0f, Alignment.center, 2));

            var childView1 = AddChildViewTo(gameObject);
            var childView2 = AddChildViewTo(gameObject);

            sut.TestApplyLayout(new Bounds2D(Vector2.zero, new Vector2(10.0f, 10.0f)));
            
            Assert.AreEqual(3.0f, childView1.Frame.size.x);
            Assert.AreEqual(4.0f, childView2.Frame.size.x);

            Assert.AreEqual(0f, childView1.Frame.origin.x);
            Assert.AreEqual(5.0f, childView2.Frame.origin.x);

            Assert.AreEqual(10.0f, childView1.Frame.size.y);
            Assert.AreEqual(10.0f, childView2.Frame.size.y);
        }
    }
}
