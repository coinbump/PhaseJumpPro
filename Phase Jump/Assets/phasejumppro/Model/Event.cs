/*
 * RATING: 5 stars. Simple type.
 * CODE REVIEW: 3.31.18
 */
namespace PJ {

	/// <summary>
	/// Any event that can occur in app
	/// </summary>
	public class Event
	{
		// OPTIONAL:
		public string key;	// Unique identifier

		public Event() {
		}

		public Event(string key) {
			this.key = key;
		}
	}

}
