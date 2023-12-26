using System.Collections;

/*
 * RATING: 5 stars.
 * Simple class
 * CODE REVIEW: 11.5.22
 * PORTED TO: C++
 */
namespace PJ {
	/// <summary>
	/// Encapsulates time delta for update events
	/// Allows us to add additional metadata for time events if needed
	/// </summary>
	public struct TimeSlice
	{
		public float delta;

		public TimeSlice(float delta) {
			this.delta = delta;
		}
	}
}
