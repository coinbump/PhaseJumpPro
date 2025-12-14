using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Simple utility
CODE REVIEW: 12/29/22
PORTED TO: Not ported (Unity only)
*/
namespace PJ
{
    public class UnityRandom : SomeRandom
    {
        public override float Value => UnityEngine.Random.value;
        public override int RangeValue(int min, int max)
        {
            return UnityEngine.Random.Range(min, max);
        }

        public override void SetSeed(int seed)
        {
            UnityEngine.Random.InitState(seed);
        }
    }
}
