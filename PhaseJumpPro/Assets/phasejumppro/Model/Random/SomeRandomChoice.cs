using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 2/18/23
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Returns a random choice of the specified type
    /// </summary>
    public abstract class SomeRandomChoice<Type>
    {
        public abstract Type Choose(SomeRandom random);
    }

    public abstract class SomeRandomIntChoice : SomeRandomChoice<int>
    {
    }

    public abstract class SomeRandomFloatChoice : SomeRandomChoice<float>
    {
    }

    /// <summary>
    /// Returns value based on min/max values
    /// </summary>
    public class MinMaxIntRandomChoice : SomeRandomIntChoice
    {
        public int min;
        public int max;

        public MinMaxIntRandomChoice(int min, int max)
        {
            this.min = min;
            this.max = max;
        }

        public override int Choose(SomeRandom random)
        {
            return min + (int)MathF.Round(((float)max - (float)min) * random.Value);
        }
    }

    /// <summary>
    /// Returns value based on min/max values
    /// </summary>
    public class MinMaxFloatRandomChoice : SomeRandomFloatChoice
    {
        public float min;
        public float max;

        public MinMaxFloatRandomChoice(float min, float max)
        {
            this.min = min;
            this.max = max;
        }

        public override float Choose(SomeRandom random)
        {
            return min + ((max - min) * random.Value);
        }
    }

    /// <summary>
    /// Returns value based on variance values
    /// </summary>
    public class VaryIntRandomChoice : SomeRandomIntChoice
    {
        public int value;
        public int vary;

        public override int Choose(SomeRandom random)
        {
            float floatValue = random.VaryFloat((float)value, (float)vary);
            return (int)MathF.Round(floatValue);
        }
    }

    /// <summary>
    /// Returns value based on variance values
    /// </summary>
    public class VaryFloatRandomChoice : SomeRandomFloatChoice
    {
        public float value;
        public float vary;

        public override float Choose(SomeRandom random)
        {
            return random.VaryFloat(value, vary);
        }
    }
}
