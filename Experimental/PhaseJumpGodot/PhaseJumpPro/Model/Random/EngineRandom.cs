using System;
using System.Collections.Generic;
using Godot;

/*
RATING: 5 stars
Simple utility
CODE REVIEW: 12/29/22
*/
namespace PJ
{
    public class EngineRandom : SomeRandom
    {
        public override float Value => GD.Randf();
        public override int RangeValue(int min, int max)
        {
            return (int)(GD.Randi() % (uint)((max - min) + 1) + (uint)min);
        }

        public override void SetSeed(int seed)
        {
            GD.Seed((uint)seed);
        }
    }
}
