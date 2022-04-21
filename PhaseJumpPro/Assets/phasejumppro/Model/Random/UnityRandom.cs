using System;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    public class UnityRandom : SomeRandom
    {
        public float Value => UnityEngine.Random.value;
    }
}
