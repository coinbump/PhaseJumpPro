using System;
using Godot;
using System.Collections.Generic;

// PORT: TODO
namespace PJ
{
    /// <summary>
    /// Returns value based on list, and optionally removes choice from list
    /// </summary>
    public class ListRandomChoice<Type> : SomeRandomChoice<Type>
    {
        public enum ChoiceBehavior
        {
            // Keep choice
            Keep,

            // Remove choice after choosing it
            Remove
        }

        public List<Type> choices = new();
        public ChoiceBehavior choiceBehavior;

        public ListRandomChoice(ChoiceBehavior choiceBehavior)
        {
            this.choiceBehavior = choiceBehavior;
        }

        public ListRandomChoice(List<Type> choices, ChoiceBehavior choiceBehavior)
        {
            this.choices = choices;
            this.choiceBehavior = choiceBehavior;
        }

        public override Type Choose(SomeRandom random)
        {
            if (choices.Count == 0) { return default; }

            int index = random.RangeValue(0, choices.Count);
            var result = choices[index];

            switch (choiceBehavior)
            {
                case ChoiceBehavior.Keep:
                    break;
                case ChoiceBehavior.Remove:
                    choices.RemoveAt(index);
                    break;
            }

            return result;
        }
    }
}
