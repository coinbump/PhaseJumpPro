using System;
using System.Collections.Generic;
using UnityEngine;

// PORT: TODO
namespace PJ
{
    /// <summary>
    /// Make N random choices
    /// </summary>
    public class SomeRandomMultiChoice<Type>
    {
        SomeRandomIntChoice countChoice;
        SomeRandomChoice<Type> choice;

        public SomeRandomMultiChoice(SomeRandomIntChoice countChoice, SomeRandomChoice<Type> choice)
        {
        }

        public List<Type> Choose(SomeRandom random)
        {
            if (null == countChoice || null == choice) { return new(); }

            var count = countChoice.Choose(random);
            if (0 == count) { return new(); }

            List<Type> result = new();

            for (int i = 0; i < count; i++)
            {
                var choice = this.choice.Choose(random);
                result.Add(choice);
            }

            return result;
        }
    }
}
