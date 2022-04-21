using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

/*
 * RATING: 5 stars
 * Utility class
 * CODE REVIEW: 4/18/22
 */
namespace PJ
{
	/// <summary>
	/// Keeps track of objects that are alive without keeping strong references
	/// For emitters/spawners that need to keep track of the objects they've spawned
	/// </summary>
	public class WeakGameObjectSet : HashSet<HashedWeakReference<GameObject>>
	{
		/// <summary>
		/// Call this to get rid of old/dead objects
		/// </summary>
		public void Refresh()
		{
			var removeEmits = new List<HashedWeakReference<GameObject>>();
			foreach (HashedWeakReference<GameObject> r in this)
			{
				// A) Game object is marked "dead" in script, 
				// B) Accessing its name produces an exception (the object was destroyed, but we still have a reference to it)
				// C) The WeakReference target is gone (Garbage Collector caught up)
				if (r.Reference.TryGetTarget(out GameObject go))
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

			foreach (HashedWeakReference<GameObject> r in removeEmits)
			{
				this.Remove(r);
			}
		}
	}
}
