using UnityEngine;
using System.Collections.Generic;

namespace PJ
{
	public static class Utils
	{
		public static bool CompareEqual(float a, float b, float epsilon = .001f)
		{
			return Mathf.Abs(Mathf.Abs(a) - Mathf.Abs(b)) < epsilon;
		}
	}

	public static class RandomUtils
	{
		public static float VaryFloat(float value, float vary)
		{
			bool isVaryForward = Random.value < 0.5f;
			float factor = isVaryForward ? 1.0f : -1.0f;
			float result = Mathf.Max(0, value + (UnityEngine.Random.value * vary * factor));

			return result;
		}
	}

	public static class EditorUtils
	{
		public enum RenderState
		{
			Default,
			Selected
		}

		// FUTURE: support this (but Unity can't fill circles)
		public enum ShapeStyle
		{
			Outline,
			OutlineAndArea
		}

		public static void RenderLinesBetweenObjects(List<GameObject> objects, RenderState renderState)
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
						case EditorUtils.RenderState.Default:
							Gizmos.color = Color.white;
							break;
						case EditorUtils.RenderState.Selected:
							Gizmos.color = GUI.skin.settings.selectionColor;
							break;
					}
					Gizmos.DrawLine(positionStart, positionEnd);
				}
				hasFirstPosition = true;
				prevPosition = position;
			}
		}

		public static void RenderCircleGizmo(Vector3 center, float radius, RenderState renderState)
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
	}
}
