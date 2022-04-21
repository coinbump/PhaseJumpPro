using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple type
 * CODE REVIEW: 4/13/22
 */
namespace PJ
{
	/// <summary>
	/// A signal is a collection of typed-keys (usually strings)
    /// These can be sent in response to user actions and used to trigger things, like tutorials
    /// </summary>
	public class Signal<Key>
	{
		protected HashSet<Key> keys = new HashSet<Key>();

		public bool Contains(Key key)
		{
			return keys.Contains(key);
		}

		public void AddKey(Key key)
		{
			keys.Add(key);
		}
	}
}
