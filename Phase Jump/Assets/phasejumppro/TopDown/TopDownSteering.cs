using UnityEngine;
using System.Collections;
using PJ;
using System.Collections.Generic;

namespace PJ
{
	public class TopDownSteering : MonoBehaviour
	{
		/*
		 * Only make a change to the steering if we're steering towards an obstacle
		 */
		public bool DoesCollisionAffectSteering(Collision2D collision) {

			TopDownNode2D target = GetComponent<TopDownNode2D>();
			if (null == target) { return false; }
			if (null == target.topDown) { return false; }

			Collider2D myCollider = collision.otherCollider;
			Bounds myBounds = myCollider.bounds;
			Collider2D otherCollider = collision.collider;

			bool doesColliderAffectSteering = false;

			foreach (ContactPoint2D hit in collision.contacts)
			{
				// Move inward a bit in case we hit a corner.
				Vector2 max = Vector2.Lerp(myBounds.max, myBounds.center, 0.01f);
				Vector2 min = Vector2.Lerp(myBounds.min, myBounds.center, 0.01f);

				float maxX = max.x;
				float minX = min.x;
				float maxY = max.y;
				float minY = min.y;

				switch (target.topDown.directionState.State)
				{
					case TopDown2D.DirectionState.Left:
						doesColliderAffectSteering = hit.point.x < myBounds.center.x && hit.point.y < maxY && hit.point.y > minY;
						break;
					case TopDown2D.DirectionState.Right:
						doesColliderAffectSteering = hit.point.x > myBounds.center.x && hit.point.y < maxY && hit.point.y > minY;
						break;
					case TopDown2D.DirectionState.Up:
						doesColliderAffectSteering = hit.point.y > myBounds.center.y && hit.point.x < maxX && hit.point.x > minX;
						break;
					case TopDown2D.DirectionState.Down:
						doesColliderAffectSteering = hit.point.y < myBounds.center.y && hit.point.x < maxX && hit.point.x > minX;
						break;
				}

				if (doesColliderAffectSteering)
				{
					break;
				}
			}

			return doesColliderAffectSteering;
		}

		protected virtual void Start()
		{

		}

		protected virtual void Update()
		{

		}

	}

}
