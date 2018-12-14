using System.Collections;
using System.Collections.Generic;

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
}
