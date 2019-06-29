using UnityEngine;
using System;

namespace PJ
{
	/// <summary>
	/// Rotates the Node2D to aim at the mouse
	/// </summary>
	public class AimAtMouseInputControlScheme2D : AbstractInputControlScheme
	{
		/// <summary>
		/// if null, use the associated GameObject
		/// </summary>
		[SerializeField]
		private GameObject target;

		public AimAtMouseInputControlScheme2D()
		{
		}

		public AimAtMouseInputControlScheme2D(GameObject target)
		{
			this.target = target;
		}

		/// <summary>
		/// if null, use the associated GameObject
		/// </summary>
		public virtual void EvtUpdate(TimeSlice time)
		{
			var target = this.target;
			if (null == target)
			{
				target = this.gameObject;
			}
			if (null == target) { return; }

			Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

			var node = target.GetComponent<Node2D>();
			if (null == node) {
				Debug.LogWarning("Aim at Mouse requires a Node2D");
				return; 
			}

			var theRenderer = target.GetComponent<Renderer>();
			if (null == theRenderer) { return; }

			Vector3 origin3 = ray.origin;
			Vector3 centerPoint3 = theRenderer.bounds.center;
			Vector2 origin2 = new Vector2(origin3.x, origin3.y);
			Vector2 centerPoint2 = new Vector2(centerPoint3.x, centerPoint3.y);

			var angle = AngleUtils.Vector2ToDegreeAngle(new Vector2(origin2.x - centerPoint2.x, origin2.y - centerPoint2.y));
			node.RotationDegreeAngle = angle;
		}

		public void Update()
		{
			var timeSlice = new TimeSlice(Time.deltaTime);
			EvtUpdate(timeSlice);
		}
	}

}
