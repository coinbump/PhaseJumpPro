using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

/*
 * RATING: 5 stars
 * Simple, has unit tests
 * Code review: 1/7/21
 */
namespace PJ
{
	/// <summary>
	/// Holds custom attributes (tags) in a dictionary
	/// </summary>
	public class Tags : Dictionary<string, object>
	{
		/// <summary>
		/// Creates a value if it doesn't exist
		/// </summary>
		public T SafeValue<T>(string key) where T : new()
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

		public T SafeValue<T>(string key, Func<T> constructor)
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

			return constructor();
		}

		/// <summary>
		/// Returns true if the value exists, and is of the correct type
		/// </summary>
		public bool ContainsTypedValue<T>(string key) where T : new()
		{
			try
			{
				T result = (T)this[key];
				return true;
			}
			catch
			{
			}

			return false;
		}

		public bool ContainsTypedValue<T>(string key, Func<T> constructor)
		{
			try
			{
				T result = (T)this[key];
				return true;
			}
			catch
			{
			}

			return false;
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
