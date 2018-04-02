using System.Collections;

/*
 * RATING: 5 stars. Simple type.
 * CODE REVIEW: 3.31.18
 */
namespace PJ {

	/// <summary>
	/// Key-value for encoding to JSON/XML
	/// </summary>
	public class Attribute {
		public string key;
		public string value;

		public Attribute(string key, string value) {
			this.key = key;
			this.value = value;
		}

		// OPTIONAL:
		public AttributeType type = AttributeType.Default;

		public bool Equals(Attribute b) {
			return key == b.key; // WRONG: && value == b.value;
		}
	}

}
