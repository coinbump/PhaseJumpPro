using System;
using System.Collections;
using System.Collections.Generic;
using PJ;

/*
 * RATING: 5 stars
 * Utilities
 * CODE REVIEW: 4/15/22
 */
public static class ListExtensions
{
    public static void Shuffle<T>(this IList<T> list, SomeRandom random)
    {
        int n = list.Count;
        while (n > 1)
        {
            n--;
            int k = random.IntValue(n + 1);
            T value = list[k];
            list[k] = list[n];
            list[n] = value;
        }
    }
}
