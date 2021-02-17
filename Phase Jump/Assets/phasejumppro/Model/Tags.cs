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
	/// </summary>
	public class TypeTags : HashSet<string>
	{
		public TypeTags() : base()
		{

		}

		public TypeTags(IEnumerable<string> collection) : base(collection)
		{
		}
	}
}

