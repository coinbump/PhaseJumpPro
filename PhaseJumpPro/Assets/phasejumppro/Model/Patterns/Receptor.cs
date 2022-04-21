using System.Collections;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simpel design pattern
 * CODE REVIEW: 4/14/22
 */
namespace PJ {
	/// <summary>
	/// A receptor responds to signals. A receptor is activated when the signal matches
	/// a key that the receptor responds to
	/// </summary>
	public abstract class Receptor<Key>
	{
		protected abstract void OnSignalMatch(Signal<Key> signal);

		public HashSet<Key> locks = new HashSet<Key>();

		public void AddLockForKey(Key key)
		{
			locks.Add(key);
		}

		public virtual bool IsMatch(Signal<Key> signal)
		{
			bool result = false;

			foreach (Key key in locks) {
				if (signal.Contains(key)) {
					result = true;
					break;
				}
			}

			return result;

		}

		public void OnSignal(Signal<Key> signal)
		{
			if (IsMatch(signal))
			{
				OnSignalMatch(signal);
			}
		}

		public bool Contains(Key key)
		{
			return locks.Contains(key);
		}
	}
}
