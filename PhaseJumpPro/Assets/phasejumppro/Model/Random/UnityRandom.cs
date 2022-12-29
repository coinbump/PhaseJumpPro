using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Simple utility
CODE REVIEW: 12/29/22
*/
namespace PJ
{
    public class UnityRandom : SomeNormalRandom
    {
        public float Value => UnityEngine.Random.value;
    }
}
