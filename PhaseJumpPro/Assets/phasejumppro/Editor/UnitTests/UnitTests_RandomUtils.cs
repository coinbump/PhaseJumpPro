using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_RandomUtils
    {
        [Test]
        public void UnitTests()
        {
            string[] choices = new[] { "a", "b", "c" };

            for (int i = 0; i < choices.Length; i++)
            {
                var choice = new UnityRandom().ChooseFrom(choices);
                Debug.Log("UnitTests_new UnityRandom().Choice: " + choice);
                Assert.IsTrue(choice.Contains(choice));
            }
        }
    }
}
