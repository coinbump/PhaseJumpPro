using System.Collections.Generic;

namespace PJ
{
    /// <summary>
    /// Pick an element in sequential order
    /// </summary>
    public class SequenceSelector<Choice> : SomeSelector<Choice>
    {
        public List<Choice> choices = new();
        public int index;

        public override Choice Select()
        {
            if (choices.Count == 0) { return default; }
            if (index >= choices.Count)
            {
                index = 0;
            }

            var result = choices[index];

            index++;

            return result;
        }
    }
}
