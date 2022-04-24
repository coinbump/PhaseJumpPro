using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple abstract class
 * CODE REVIEW: 4/16/22
 */
namespace PJ
{
	/// <summary>
	/// Some device that sends inputs (mouse, gamepad, etc.)
	/// </summary>
	public abstract class SomeInputController
	{
		public abstract bool IsAvailable();
	}
}
