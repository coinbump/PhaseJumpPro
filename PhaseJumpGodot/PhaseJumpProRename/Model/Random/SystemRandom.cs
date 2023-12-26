using System;

namespace PJ
{
    /// <summary>
    /// Uses C# system random
    /// </summary>
    public class SystemRandom : SomeRandom
    {
        public System.Random random = new();

        public override float Value => (float)random.NextDouble();
        public override int RangeValue(int min, int max)
        {
            return random.Next(min, max);
        }

        public override void SetSeed(int seed)
        {
            random = new(seed);
        }
    }
}
