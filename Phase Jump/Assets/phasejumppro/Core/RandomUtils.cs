using UnityEngine;
using System.Collections.Generic;

namespace PJ
{
	public static class RandomUtils
	{
		public static Type ChooseFrom<Type>(Type[] choices)
		{
			if (choices.Length == 0) { return default; }

			int index = Random.Range(0, choices.Length);
			return choices[index];
		}

		public static Type ChooseFrom<Type>(List<Type> choices)
        {
			if (choices.Count == 0) { return default; }

			int index = Random.Range(0, choices.Count);
			return choices[index];
		}

		public static float VaryFloat(float value, float vary)
		{
			bool isVaryForward = Random.value < 0.5f;
			float factor = isVaryForward ? 1.0f : -1.0f;
			float result = Mathf.Max(0, value + (UnityEngine.Random.value * vary * factor));

			return result;
		}

		/// <summary>
		/// Returns a random value between -delta...delta.
		/// </summary>
		/// <param name="delta">Max delta, positive or negative to be returned</param>
		/// <returns></returns>
		public static float Delta(float delta)
		{
			bool isVaryForward = Random.value < 0.5f;
			float factor = isVaryForward ? 1.0f : -1.0f;
			return Random.value * factor * delta;
		}
	}
}
