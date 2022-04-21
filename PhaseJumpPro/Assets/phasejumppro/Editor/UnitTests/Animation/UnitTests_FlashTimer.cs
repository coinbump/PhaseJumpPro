using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_FlashTimer
	{
		[Test]
		public void TestFlashTimer()
		{
			var flashTimer = new FlashTimer(2, 1.0f);

			Assert.AreEqual(false, flashTimer.IsFlashOn);

			flashTimer.OnUpdate(new TimeSlice(0.5f));
			Assert.AreEqual(false, flashTimer.IsFlashOn);

			flashTimer.OnUpdate(new TimeSlice(0.5f));
			Assert.AreEqual(true, flashTimer.IsFlashOn);

			flashTimer.OnUpdate(new TimeSlice(0.5f));
			Assert.AreEqual(true, flashTimer.IsFlashOn);

			flashTimer.OnUpdate(new TimeSlice(0.5f));
			Assert.AreEqual(false, flashTimer.IsFlashOn);

			Assert.AreEqual(false, flashTimer.IsFinished);
			flashTimer.OnUpdate(new TimeSlice(2.0f));
			Assert.AreEqual(false, flashTimer.IsFlashOn);
			Assert.AreEqual(true, flashTimer.IsFinished);
		}
	}
}
