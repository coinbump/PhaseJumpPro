using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_UISystemSelection
    {
        [Test]
        public void TestSelection()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<UISystem>();
            var selectHandler1 = gameObject.AddComponent<SelectHandler>();
            var selectHandler2 = gameObject.AddComponent<SelectHandler>();

            Assert.AreEqual(0, sut.Selection.Count);
            Assert.IsFalse(selectHandler1.IsSelected);
            Assert.IsFalse(selectHandler2.IsSelected);

            sut.Selection = new HashSet<SelectHandler> { selectHandler1 };

            Assert.AreEqual(1, sut.Selection.Count);
            Assert.IsTrue(selectHandler1.IsSelected);
            Assert.IsFalse(selectHandler2.IsSelected);

            sut.Selection = new HashSet<SelectHandler> { selectHandler2 };

            Assert.AreEqual(1, sut.Selection.Count);
            Assert.IsFalse(selectHandler1.IsSelected);
            Assert.IsTrue(selectHandler2.IsSelected);

            sut.Selection = new HashSet<SelectHandler> { selectHandler1, selectHandler2 };

            Assert.AreEqual(2, sut.Selection.Count);
            Assert.IsTrue(selectHandler1.IsSelected);
            Assert.IsTrue(selectHandler2.IsSelected);
        }

        [Test]
        public void TestUpdateSelectionFor()
        {
            var gameObject = new GameObject();
            var sut = gameObject.AddComponent<UISystem>();
            var selectHandler1 = gameObject.AddComponent<SelectHandler>();
            var selectHandler2 = gameObject.AddComponent<SelectHandler>();

            Assert.AreEqual(0, sut.Selection.Count);
            Assert.IsFalse(selectHandler1.IsSelected);
            Assert.IsFalse(selectHandler2.IsSelected);

            selectHandler1.IsSelected = true;
            sut.UpdateSelectionFor(selectHandler1);

            Assert.AreEqual(1, sut.Selection.Count);
            Assert.IsTrue(selectHandler1.IsSelected);
            Assert.IsFalse(selectHandler2.IsSelected);
        }
    }
}
