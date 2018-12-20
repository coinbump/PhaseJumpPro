using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ { 
	/// <summary>
	/// Moves objects around a circle radius
	/// FUTURE: support rotated ellipses, sin paths, etc.
	/// </summary>
	public class CirclePath2D : AbstractMovePath2D
	{
		public float radius = 3.0f;

#if UNITY_EDITOR
		protected override void RenderGizmos(EditorUtils.RenderState renderState)
		{
			EditorUtils.RenderCircleGizmo(transform.position, radius, renderState);
		}
#endif

		public override void SnapNodeToPath(Node2D node, bool force)
		{
			float progress = node.pathInfo.waypointProgress;
			float theta = -Mathf.PI * 2 * progress;
			Vector2 center = transform.position;
			node.MoveToPosition(new Vector2(center.x - radius * Mathf.Sin(theta), center.y + radius * Mathf.Cos(theta)), force);
		}

		public override void UpdateNodeOnPath(Node2D node)
		{
			float timeDelta = Time.deltaTime * speed * node.pathInfo.speed;

			if (node.pathInfo.isMovingForward) {
				node.pathInfo.waypointProgress += timeDelta;
			}
			else
			{
				node.pathInfo.waypointProgress -= timeDelta;
			}

			node.pathInfo.waypointProgress %= 1.0f;
			if (node.pathInfo.waypointProgress < 0)
			{
				node.pathInfo.waypointProgress = 1.0f + node.pathInfo.waypointProgress;
			}
		}
	}

}
