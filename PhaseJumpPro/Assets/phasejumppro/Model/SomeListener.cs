using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple design pattern
 * CODE REVIEW: 4/13/22
 */
namespace PJ {
	/// <summary>
	/// Listeners are tightly coupled with broadcasters
	/// </summary>
	public interface SomeListener
	{
		public void OnListen(Event theEvent);
	}
}
