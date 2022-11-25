using System;

/*
 * RATING: 5 stars
 * Simple type
 * CODE REVIEW: 4/13/22
 * 
 * PORTED TO: C++
 */
namespace PJ {

	/// <summary>
	/// Used to send event from one object to another
	/// </summary>
    /// Naming convention: EventName or Events.Name. Avoids polluting the namespace with
    /// a large amount of event objects.
	public class Event
	{
		// OPTIONAL:
		public WeakReference<object> sentFrom;

		/// <summary>
        /// Event id
        /// </summary>
		public string id;
		
		public Event() {
		}

		public Event(string id) {
			this.id = id;
		}

		public Event(string id, object sentFrom)
		{
			this.id = id;
			this.sentFrom = new WeakReference<object>(sentFrom);
		}
	}
}
