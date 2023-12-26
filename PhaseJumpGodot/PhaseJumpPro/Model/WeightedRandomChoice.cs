using System.Collections;
using System.Collections.Generic;
using Godot;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 2/18/23
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Each choice has a weight that affects its random probability
    /// Useful for cases where we want randomness, but prefer certain outcomes
    /// </summary>
    public class WeightedRandomChoice<Value> : SomeRandomChoice<Value>
    {
        public List<Weight<Value>> choices = new();

        public WeightedRandomChoice()
        {
        }

        public void Add(Weight<Value> choice)
        {
            choices.Add(choice);
        }

        public override Value Choose(SomeRandom random)
        {
            var weight = ChooseWeight(random);
            return weight != null ? weight.value : default;
        }

        public Weight<Value> ChooseWeight(SomeRandom random)
        {
            float factor = random.Value;
            var result = ChooseWeightAt(factor);
            return result;
        }

        public Weight<Value> ChooseWeightAt(float factor)
        {
            List<Weight<Value>> adjustedWeights = new();

            float totalWeight = 0;
            foreach (var wr in choices)
            {
                var adjustedWeight = wr.WeightValue;
                if (null != wr.adjust)
                {
                    adjustedWeight = wr.adjust.Transform(wr).WeightValue;
                }

                adjustedWeights.Add(new Weight<Value>(adjustedWeight, wr.value));

                totalWeight += adjustedWeight;
            }

            // No viable choices
            if (totalWeight.Equals(0))
            {
                return null;
            }

            float random = totalWeight * factor;

            float curWeight = 0;
            foreach (var wr in adjustedWeights)
            {
                float weight = wr.WeightValue;
                if (weight <= 0)
                {
                    continue;   // Ignore this, invalid
                }

                curWeight += weight;
                if (random <= curWeight)
                {
                    return wr;
                }
            }

            return null;
        }
    }

    //     public WeightedRandomChoice<Value> : SomeRandomChoice
    //     {
    //         public WeightedRandom<Value> random = new();

    // }
}
