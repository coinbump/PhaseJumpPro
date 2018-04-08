using System.Collections;
using System.Collections.Generic;
using NUnit.Framework;

namespace PJ {
	/// <summary>
	/// A receptor responds to signals. A receptor is activated when the signal matches
	/// a key that the receptor responds to
	/// </summary>
	public abstract class Receptor<Key>
	{
		protected abstract void EvtSignalMatched(Signal<Key> signal);

		public HashSet<Key> locks = new HashSet<Key>();

		public void AddLockForKey(Key key)
		{
			locks.Add(key);
		}

		public virtual bool DoesSignalMatch(Signal<Key> signal)
		{
			bool result = false;

			foreach (Key key in locks) {
				if (signal.MatchKey(key)) {
					result = true;
					break;
				}
			}

			return result;

		}

		public void EvtSignal(Signal<Key> signal)
		{
			if (DoesSignalMatch(signal))
			{
				EvtSignalMatched(signal);
			}
		}

		public bool MatchKey(Key key)
		{
			return locks.Contains(key);
		}
	}

	public class UnitTests_SignalReceptor {
		class TestReceptor : Receptor<string> {
			public int matchCount;

			public TestReceptor() {
				AddLockForKey("a");
				AddLockForKey("c");
			}

			protected override void EvtSignalMatched(Signal<string> signal) {
				matchCount++;
			}
		}

		class TestSignalMatch : Signal<string> {
			public TestSignalMatch() {
				AddKey("a");
				AddKey("b");
			}
		}

		class TestSignalNoMatch : Signal<string>
		{
			public TestSignalNoMatch()
			{
				AddKey("d");
			}
		}

		[Test]
		public void UnitTests() {
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
