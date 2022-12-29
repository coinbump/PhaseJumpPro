using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/21/22
 */
namespace PJ
{
    /// <summary>
    /// Each choice has a weight that affects its random probability
    /// Useful for cases where we want randomness, but prefer certain outcomes
    /// </summary>
    public class WeightedRandom : List<Weight>
    {
        /// <summary>
        /// Injected random for testability
        /// </summary>
        protected SomeNormalRandom random;

        public WeightedRandom(SomeNormalRandom random)
        {
            this.random = random;
        }

        public Weight ChooseRandom()
        {
            float factor = random.Value;
            var result = ChooseFactor(factor);
            return result;
        }

        public Weight ChooseFactor(float factor)
        {
            List<Weight> adjustedWeights = new List<Weight>();

            float totalWeight = 0;
            foreach (Weight wr in this)
            {
                var adjustedWeight = wr.Value;
                if (null != wr.adjust)
                {
                    adjustedWeight = wr.adjust.Transform(wr).Value;
                }

                adjustedWeights.Add(new Weight(adjustedWeight, wr));

                totalWeight += adjustedWeight;
            }

            // No viable choices
            if (totalWeight.Equals(0))
            {
                return null;
            }

            float random = totalWeight * factor;

            float curWeight = 0;
            foreach (Weight wr in adjustedWeights)
            {
                float weight = wr.Value;
                if (weight <= 0)
                {
                    continue;   // Ignore this, invalid
                }

                curWeight += weight;
                if (random <= curWeight)
                {
                    return (Weight)wr.target;
                }
            }

            return null;
        }
    }
}
