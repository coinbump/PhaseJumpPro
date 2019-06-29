using UnityEngine;
using System;

namespace PJ
{
	/// <summary>
	/// Moves the center of the gameObject to the mouse position
	/// </summary>
	public class MoveToMouseInputControlScheme2D : AbstractInputControlScheme
	{
		private MouseInputController mouseInputController;

		private void Awake()
		{
			mouseInputController = new MouseInputController();
		}

		public MoveToMouseInputControlScheme2D()
		{
		}

		public virtual void EvtUpdate(TimeSlice time)
		{
			if (null == mouseInputController || !mouseInputController.IsAvailable()) { return; }

			var target = this.gameObject;
			if (null == target) { return; }

			Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

			var theRenderer = target.GetComponent<Renderer>();
			if (null == theRenderer) { return; }

			Vector3 origin3 = ray.origin;
			Vector3 centerPoint3 = theRenderer.bounds.center;
			Vector2 origin2 = new Vector2(origin3.x, origin3.y);
			Vector2 centerPoint2 = new Vector2(centerPoint3.x, centerPoint3.y);

			float width = theRenderer.bounds.size.x;
			float height = theRenderer.bounds.size.y;
			float newX = origin2.x - (width / 2.0f);
			float newY = origin2.y - (height / 2.0f);

			target.transform.position = new Vector3(newX, newY, target.transform.position.z);
		}

		public void Update()
		{
			var timeSlice = new TimeSlice(Time.deltaTime);
			EvtUpdate(timeSlice);
		}
	}

}
