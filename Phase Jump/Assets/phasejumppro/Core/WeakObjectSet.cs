using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

/*
 * RATING: 4 stars.
 * CODE REVIEW: 12/19/18
 * 
 * TODO: could use a unit test.
 */
namespace PJ
{
	/// <summary>
	/// Keeps track of objects that are alive without keeping strong references
	/// For emitters/spawners that need to keep track of the objects they've spawned
	/// </summary>
	public class WeakObjectSet : HashSet<WeakReference<GameObject>>
	{
		/// <summary>
		/// Call this to get rid of old/dead objects
		/// </summary>
		public void Refresh()
		{
			var removeEmits = new List<WeakReference<GameObject>>();
			foreach (WeakReference<GameObject> r in this)
			{
				// Either A) a game object is marked "dead" in script, 
				// or B) accessing its name produces an exception (means the object was destroyed)
				// or C) the WeakReference target is gone (Garbage Collector caught up).
				// For best performance, we have to check all 3.
				if (r.TryGetTarget(out GameObject go))
				{
					try
					{
						if (go.name == "_dead_")
						{
							removeEmits.Add(r);
						}
					}
					catch
					{
						removeEmits.Add(r);
					}
				}
				else
				{
					removeEmits.Add(r);
				}
			}

			foreach (WeakReference<GameObject> r in removeEmits)
			{
				this.Remove(r);
			}
		}
	}
}
