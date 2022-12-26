using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_FocusCoordinator
    {
        [Test]
        public void Test_FocusablePriorityComparer()
        {
            var gameObject = new GameObject();
            var focusable1 = gameObject.AddComponent<FocusHandler>();
            var focusable2 = gameObject.AddComponent<FocusHandler>();

            focusable1.focusPriority = 0;
            focusable2.focusPriority = 10;

            var focusables = new FocusHandler[] { focusable1, focusable2 };
            Assert.AreEqual(focusable1, focusables[0]);
            Assert.AreEqual(focusable2, focusables[1]);

            var sortedFocusables = new List<FocusHandler>(focusables);
            sortedFocusables.Sort(new FocusablePriorityComparer());

            Assert.AreEqual(focusable2, sortedFocusables[0]);
            Assert.AreEqual(focusable1, sortedFocusables[1]);
        }

        [Test]
        public void Test_FocusableMoveComparer_SameAngle_Priority_Sort()
        {
            var gameObject1 = new GameObject();
            var focusable1 = gameObject1.AddComponent<FocusHandler>();

            var gameObject2 = new GameObject();
            var focusable2 = gameObject2.AddComponent<FocusHandler>();

            focusable1.transform.position = new Vector3(0, 1, 0);
            focusable2.transform.position = new Vector3(0, 1, 0);

            focusable1.focusPriority = 0;
            focusable2.focusPriority = 10;

            var focusables = new FocusHandler[] { focusable1, focusable2 };
            Assert.AreEqual(focusable1, focusables[0]);
            Assert.AreEqual(focusable2, focusables[1]);

            var sortedFocusables = new List<FocusHandler>(focusables);
            sortedFocusables.Sort(new FocusableMoveComparer(new Vector3(0, 0, 0), FocusDirection.Up));

            Assert.AreEqual(focusable2, sortedFocusables[0]);
            Assert.AreEqual(focusable1, sortedFocusables[1]);
        }

        [Test]
        public void Test_FocusableMoveComparer_SameAngle_Distance_Sort()
        {
            var gameObject1 = new GameObject();
            var focusable1 = gameObject1.AddComponent<FocusHandler>();

            var gameObject2 = new GameObject();
            var focusable2 = gameObject2.AddComponent<FocusHandler>();

            focusable1.transform.position = new Vector3(0, 1, 0);
            focusable2.transform.position = new Vector3(0, 0.5f, 0);

            var focusables = new FocusHandler[] { focusable1, focusable2 };
            Assert.AreEqual(focusable1, focusables[0]);
            Assert.AreEqual(focusable2, focusables[1]);

            var sortedFocusables = new List<FocusHandler>(focusables);
            sortedFocusables.Sort(new FocusableMoveComparer(new Vector3(0, 0, 0), FocusDirection.Up));

            Assert.AreEqual(focusable2, sortedFocusables[0]);
            Assert.AreEqual(focusable1, sortedFocusables[1]);
        }

        [Test]
        public void Test_FocusableMoveComparer_OutOfAngleRange()
        {
            var gameObject1 = new GameObject();
            var focusable1 = gameObject1.AddComponent<FocusHandler>();

            var gameObject2 = new GameObject();
            var focusable2 = gameObject2.AddComponent<FocusHandler>();

            focusable1.transform.position = new Vector3(1, 0, 0);
            focusable2.transform.position = new Vector3(0, 5.0f, 0);

            var focusables = new FocusHandler[] { focusable1, focusable2 };
            Assert.AreEqual(focusable1, focusables[0]);
            Assert.AreEqual(focusable2, focusables[1]);

            var sortedFocusables = new List<FocusHandler>(focusables);
            sortedFocusables.Sort(new FocusableMoveComparer(new Vector3(0, 0, 0), FocusDirection.Up));

            Assert.AreEqual(focusable2, sortedFocusables[0]);
        }
    }
}
