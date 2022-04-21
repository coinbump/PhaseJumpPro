using UnityEngine;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 3/12/22
 */
namespace PJ
{
    namespace Graph
    {
        public class RandomTypeChoiceNode : SomeTypeChoiceNode
        {
            public HashSet<string> choices = new HashSet<string>();

            public override HashSet<string> Choices => choices;
            public override string Choose()
            {
                return RandomUtils.ChooseFrom(new List<string>(choices));
            }

            public override object ValueForOutput<T>(string identifier, Tags parameters, T defaultValue)
            {
                if (typeof(T).IsAssignableFrom(typeof(string)))
                {
                    return Choose();
                }

                return defaultValue;
            }
        }
    }
}
