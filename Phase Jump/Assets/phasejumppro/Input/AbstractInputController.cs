using System;

namespace PJ
{
	/// <summary>
	/// Some device that sends inputs
	/// </summary>
	public abstract class AbstractInputController
	{
		public abstract bool IsAvailable();
	}
}
