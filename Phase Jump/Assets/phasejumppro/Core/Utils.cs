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

		public static RaycastHit2D[] Raycast2DHitsAtScreenPosition(Vector2 position)
        {
			return Physics2D.RaycastAll(Camera.main.ScreenToWorldPoint(position), Vector2.zero);
		}
	}
}
