using UnityEngine;
using System;
using UnityEditor;

/*
 * RATING: 4 stars
 * Needs unit tests, better episolon default values?
 * CODE REVIEW: 4/8/22
 */
namespace PJ
{
	public static class Utils
	{
		public const float FloatEpsilon = 1.192092896e-07F;

		public static bool Equals(float a, float b, float epsilon = FloatEpsilon)
		{
			return Mathf.Abs(Mathf.Abs(a) - Mathf.Abs(b)) < epsilon;
		}

		// FUTURE: need better epsilon default here
		public static bool Equals(double a, double b, double epsilon = .00001)
		{
			return Math.Abs(Math.Abs(a) - Math.Abs(b)) < epsilon;
		}

		// FUTURE: need tests for this
		public static RaycastHit2D[] Raycast2DHitsAtScreenPosition(Vector2 position)
        {
			return Physics2D.RaycastAll(Camera.main.ScreenToWorldPoint(position), Vector2.zero);
		}

		public static Vector2Int ScreenPixelSize()
        {
#if UNITY_EDITOR
            // The unity editor does not return correct results via Screen.width/.height
            string[] res = UnityStats.screenRes.Split('x');
            return new Vector2Int(int.Parse(res[0]), int.Parse(res[1]));
            // Screen.currentResolution returns the *entire* resolution of the monitor
#else
			return new Vector2Int(Screen.width, Screen.height);
#endif
		}

		public static Vector2 ScreenWorldSize()
        {
			var halfScreenHeight = Camera.main.orthographicSize;
			var screenHeight = halfScreenHeight * 2.0f;
			var ratio = (float)Screen.width / (float)Screen.height;
			var xSize = screenHeight * ratio;
			var ySize = screenHeight;

			return new Vector2(xSize, ySize);
		}
	}
}
