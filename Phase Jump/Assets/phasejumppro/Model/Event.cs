﻿using System;

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
		public WeakReference sentFrom;
		public string name;	// Unique identifier
		public int value;	// Unique value

		public Event() {
		}

		public Event(string name) {
			this.name = name;
		}

		public Event(int value) {
			this.value = value;
		}
	}
}
