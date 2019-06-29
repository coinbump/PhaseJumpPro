using UnityEngine;
using System;

namespace PJ
{
	/// <summary>
	/// Records mouse position on update
	/// </summary>
	public class MouseInputController : AbstractInputController
	{
		public Vector3 position { get; protected set; }
		public Ray ray { get; protected set; }

		public virtual void EvtUpdate(TimeSlice time)
		{
			position = Input.mousePosition;
			ray = Camera.main.ScreenPointToRay(Input.mousePosition);
		}
	}
}
