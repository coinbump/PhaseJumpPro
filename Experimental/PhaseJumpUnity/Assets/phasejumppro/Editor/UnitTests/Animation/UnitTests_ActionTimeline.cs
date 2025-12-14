using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_ActionTimeline
    {
        [Test]
        public void Test_Action()
        {
            var value = 0;
            var sut = new ActionTimeline();
            sut.Add(new ActionKeyframe(0, () => value++));
            sut.Add(new ActionKeyframe(1.0f, () => value++));
            sut.Add(new ActionKeyframe(2.0f, () => value++));

            Assert.AreEqual(0, value);
            sut.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(1, value);

            sut.OnUpdate(new TimeSlice(0.1f));
            Assert.AreEqual(1, value);

            sut.OnUpdate(new TimeSlice(3.0f));
            Assert.AreEqual(3, value);
        }
    }
}
