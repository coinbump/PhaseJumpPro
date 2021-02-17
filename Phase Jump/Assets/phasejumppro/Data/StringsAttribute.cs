using System.Collections;

/*
 * RATING: 5 stars. Simple type.
 * CODE REVIEW: 3.31.18
 */
namespace PJ {

	/// <summary>
	/// Key-value for encoding to JSON/XML
	/// </summary>
	public class StringsAttribute : Attribute<string, string>
	{
		public StringsAttribute(string key, string value)
			: base(key, value)
		{
		}
	}
}
