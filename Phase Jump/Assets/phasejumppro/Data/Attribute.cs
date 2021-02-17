using System;
using System.Collections;

/*
 * RATING: 5 stars. Simple type.
 * CODE REVIEW: 2.25.21
 */
namespace PJ
{
	/// <summary>
	/// Key-value for encoding to JSON/XML
	/// </summary>
	public class Attribute<Key, Value> where Key: IComparable
	{
		public Key key;
		public Value value;

		public Attribute(Key key, Value value)
		{
			this.key = key;
			this.value = value;
		}

		// OPTIONAL:
		public AttributeType type = AttributeType.Default;

		public bool Equals(Attribute<Key, Value> b)
		{
			return key.Equals(b.key); // WRONG: && value == b.value;
		}
	}
}
