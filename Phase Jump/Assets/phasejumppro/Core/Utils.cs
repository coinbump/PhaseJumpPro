using UnityEngine;
using System.Collections;

namespace PJ
{
	public static class Utils
	{
		public static bool CompareEqual(float a, float b, float epsilon = .001f)
		{
			return Mathf.Abs(Mathf.Abs(a) - Mathf.Abs(b)) < epsilon;
		}
	}
}
