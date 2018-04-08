using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
	/// <summary>
	/// Signals represent events that bind to a receptor
	/// Allows you to express and respond to user behavior categorically
	/// (useful for responding to user actions in tutorials)
	/// </summary>
	public class Signal<Key>
	{
		protected HashSet<Key> keys = new HashSet<Key>();

		public bool MatchKey(Key key) 
		{
			return keys.Contains(key);
		}

		public void AddKey(Key key)
		{
			keys.Add(key);
		}
	}

}
