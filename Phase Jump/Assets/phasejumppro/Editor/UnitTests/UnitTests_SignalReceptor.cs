using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_SignalReceptor
	{
		private class TestReceptor : Receptor<string>
		{
			public int matchCount;

			public TestReceptor()
			{
				AddLockForKey("a");
				AddLockForKey("c");
			}

			protected override void EvtSignalMatched(Signal<string> signal)
			{
				matchCount++;
			}
		}

		private class TestSignalMatch : Signal<string>
		{
			public TestSignalMatch()
			{
				AddKey("a");
				AddKey("b");
			}
		}

		private class TestSignalNoMatch : Signal<string>
		{
			public TestSignalNoMatch()
			{
				AddKey("d");
			}
		}

		[Test]
		public void UnitTests()
		{
			var signal = new TestSignalMatch();
			var receptor = new TestReceptor();
			receptor.EvtSignal(signal);
			Assert.AreEqual(1, receptor.matchCount);

			var signalNoMatch = new TestSignalNoMatch();
			receptor.EvtSignal(signalNoMatch);
			Assert.AreEqual(1, receptor.matchCount);
		}
	}
}
