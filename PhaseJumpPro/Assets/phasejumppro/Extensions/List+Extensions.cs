using System;
using System.Collections;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Utilities
 * CODE REVIEW: 4/15/22
 */
public static class ListExtensions
{
	private static Random rng = new Random();

	public static void Shuffle<T>(this IList<T> list)
	{
		int n = list.Count;
		while (n > 1)
		{
			n--;
			int k = rng.Next(n + 1);
			T value = list[k];
			list[k] = list[n];
			list[n] = value;
		}
	}
}
