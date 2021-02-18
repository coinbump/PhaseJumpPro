using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

namespace PJ
{
	/// <summary>
	/// Holds custom attributes (tags) in a dictionary
	/// </summary>
	public class Tags: Dictionary<string, object>
	{
		public T Value<T>(string key) where T : new()
		{
			try
			{
				T result = (T)this[key];
				return result;
			}
			catch (System.Exception e)
			{
				Debug.Log(e.Message);
			}

			return new T();
		}
	}

	/// <summary>
	/// Holds a set of type tag strings
    ///
    /// Example: "enemy", "ghost" tags for an arcade game
	/// </summary>
	public class TypeTagsSet : HashSet<string>
	{
		public TypeTagsSet() : base()
		{
		}

		public TypeTagsSet(IEnumerable<string> collection) : base(collection)
		{
		}
	}
}

