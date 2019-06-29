/*
 * RATING: 5 stars. Simple class
 * CODE REVIEW: 3.31.18
 */
using System.Collections;

namespace PJ {
	/// <summary>
	/// Encapsulates time delta for update events
	/// Allows for future time delta logic vs a standard float
	/// </summary>
	public struct TimeSlice
	 {
		public float delta;

		public TimeSlice(float delta) {
			this.delta = delta;
		}

	}
		
}
