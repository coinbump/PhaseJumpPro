using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace PJ
{
	/// <summary>
	/// Holds custom attributes (tags) in a dictionary
	/// </summary>
	public class Tagged: Dictionary<string, object>
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
}

