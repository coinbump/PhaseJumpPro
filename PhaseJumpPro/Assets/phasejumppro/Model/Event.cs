using System;

/*
 * RATING: 5 stars
 * Simple type
 * CODE REVIEW: 4/13/22
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
        /// Event identifier
        /// </summary>
		public string identifier;
		
		public Event() {
		}

		public Event(string identifier) {
			this.identifier = identifier;
		}

		public Event(string identifier, object sentFrom)
		{
			this.identifier = identifier;
			this.sentFrom = new WeakReference<object>(sentFrom);
		}
	}
}
