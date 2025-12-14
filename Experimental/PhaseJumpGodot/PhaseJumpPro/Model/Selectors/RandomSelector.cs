using System.Collections.Generic;

namespace PJ
{
    /// <summary>
    /// Pick a random element
    /// </summary>
    public class RandomSelector<Choice> : SomeSelector<Choice>
    {
        public List<Choice> choices = new();
        public SomeRandom random = new EngineRandom();

        public override Choice Select()
        {
            return random.ChooseFrom(choices);
        }
    }
}
