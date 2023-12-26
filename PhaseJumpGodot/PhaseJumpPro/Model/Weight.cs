using System;

/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 2/18/23
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Specifies the weight of a random event
    /// </summary>
    public class Weight<Value>
    {
        /// <summary>
        /// Base weight (never changes)
        /// </summary>
        protected float weight = 1.0f;

        public Value value;

        /// <summary>
        /// Optional object that dynamically adjusts the weight during evaluation
        /// </summary>
        public SomeValueTransform<Weight<Value>> adjust;

        public Weight(float weight)
        {
            this.weight = weight;
        }

        public Weight(float weight, Value value)
        {
            this.weight = weight;
            this.value = value;
        }

        public float WeightValue
        {
            get => weight;
            set => weight = value;
        }
    }
}
