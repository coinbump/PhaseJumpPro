﻿using System.Collections.Generic;

/*
 * RATING: 5 stars. Simple design pattern
 * CODE REVIEW: 4.7.17
 */
namespace PJ {
	/// <summary>
	/// Listeners are tightly coupled with broadcasters
	/// </summary>
	public abstract class Listener
	{
		public virtual void EvtListen(Event theEvent) {
		}
	}

}



