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
		private enum RenderState
		{
			Default,
			Selected
		}

		public float radius = 3.0f;

		private void RenderGizmos(RenderState renderState)
		{
			Vector3 center = transform.position;

			switch (renderState)
			{
				case RenderState.Default:
					Gizmos.color = Color.white;
					break;
				case RenderState.Selected:
					Gizmos.color = GUI.skin.settings.selectionColor;
					break;
			}

			float theta = 0.0f;
			float x = radius * Mathf.Cos(theta);
			float y = radius * Mathf.Sin(theta);
			Vector3 pos = center + new Vector3(x, y, 0);
			Vector3 newPos = pos;
			Vector3 lastPos = pos;
			for (theta = 0.1f; theta < Mathf.PI * 2; theta += 0.1f)
			{
				x = radius * Mathf.Cos(theta);
				y = radius * Mathf.Sin(theta);
				newPos = center + new Vector3(x, y, 0);
				Gizmos.DrawLine(pos, newPos);
				pos = newPos;
			}

			Gizmos.DrawLine(pos, lastPos);
		}

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

		void OnDrawGizmos()
		{
			RenderGizmos(RenderState.Default);
		}

		private void OnDrawGizmosSelected()
		{
			RenderGizmos(RenderState.Selected);
		}
	}

}
