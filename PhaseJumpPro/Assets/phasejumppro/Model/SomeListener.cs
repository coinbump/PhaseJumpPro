using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple design pattern
 * CODE REVIEW: 4/13/22
 * PORTED TO: C++
 */
namespace PJ {
	/// <summary>
	/// Receives events from broadcasters
	/// </summary>
	public interface SomeListener
	{
		public void OnListen(Event _event);
	}
}
