/*
 * RATING: 5 stars.
 * Simple class
 * CODE REVIEW: 11/5/22
 */
namespace PJ {
	/// <summary>
	/// Encapsulates time delta for update events
	/// Allows us to add additional metadata for time events if needed
	/// </summary>
	struct TimeSlice
	{
    public:
		float delta;

		TimeSlice(float delta) : delta(delta) {
		}
    };
}
