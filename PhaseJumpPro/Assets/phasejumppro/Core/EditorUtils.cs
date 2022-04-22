using UnityEngine;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Editor only
 * CODE REVIEW: 4/21/22
 */
namespace PJ
{
	public static class EditorUtils
	{
		public enum RenderState
		{
			Default,
			Selected
		}

		public static void DrawConnectingLines(List<GameObject> objects, RenderState renderState)
		{
			if (objects.Count == 0) { return; }

			bool hasFirstPosition = false;
			Vector3 prevPosition = new Vector3();
			Vector3 positionStart, positionEnd;

			foreach (GameObject go in objects)
			{
				Vector3 position = go.transform.position;
				positionStart = prevPosition;
				positionEnd = position;

				if (hasFirstPosition)
				{
					switch (renderState)
					{
						case RenderState.Default:
							Gizmos.color = Color.white;
							break;
						case RenderState.Selected:
							Gizmos.color = GUI.skin.settings.selectionColor;
							break;
					}
					Gizmos.DrawLine(positionStart, positionEnd);
				}
				hasFirstPosition = true;
				prevPosition = position;
			}
		}

		public static void DrawLine(Vector3 start, Vector3 end, RenderState renderState)
        {
			switch (renderState)
			{
				case RenderState.Default:
					Gizmos.color = Color.white;
					break;
				case RenderState.Selected:
					Gizmos.color = GUI.skin.settings.selectionColor;
					break;
			}

			Gizmos.DrawLine(start, end);
		}

		public static void DrawCircle(Vector3 center, float radius, RenderState renderState)
		{
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

		public static void DrawRect(Vector3 center, float width, float height, RenderState renderState)
		{
			switch (renderState)
			{
				case RenderState.Default:
					Gizmos.color = Color.white;
					break;
				case RenderState.Selected:
					Gizmos.color = GUI.skin.settings.selectionColor;
					break;
			}

			float halfWidth = width / 2.0f;
			float halfHeight = height / 2.0f;
			Vector3 topLeft = new Vector3(center.x - halfWidth, center.y - halfHeight, 0);
			Vector3 topRight = new Vector3(center.x + halfWidth, center.y - halfHeight, 0);
			Vector3 bottomRight = new Vector3(center.x + halfWidth, center.y + halfHeight, 0);
			Vector3 bottomLeft = new Vector3(center.x - halfWidth, center.y + halfHeight, 0);

			Gizmos.DrawLine(topLeft, topRight);
			Gizmos.DrawLine(topRight, bottomRight);
			Gizmos.DrawLine(bottomRight, bottomLeft);
			Gizmos.DrawLine(bottomLeft, topLeft);
		}
	}
}
