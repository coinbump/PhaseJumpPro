using UnityEngine;
using System;

namespace PJ
{
	/// <summary>
	/// Moves the center of the gameObject to the mouse position
	/// </summary>
	public class MoveToMouseInputControlScheme2D : SomeInputControlScheme
	{
		private MouseInputController mouseInputController;

		private void Awake()
		{
			if (null == mouseInputController) { 
				mouseInputController = new MouseInputController();
			}
		}

		public MoveToMouseInputControlScheme2D()
		{
			mouseInputController = new MouseInputController();
		}

		/// <summary>
		/// On update, move at the mouse (for cursors)
		/// </summary>
		public virtual void EvtUpdate(TimeSlice time)
		{
			if (null == mouseInputController || !mouseInputController.IsAvailable()) { return; }

			var target = this.gameObject;
			if (null == target) { return; }

			Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

			var theRenderer = target.GetComponent<Renderer>();
			if (null == theRenderer) { return; }

			Vector3 origin3 = ray.origin;
			target.transform.position = origin3;
		}

		public void Update()
		{
			var timeSlice = new TimeSlice(Time.deltaTime);
			EvtUpdate(timeSlice);
		}
	}
}
