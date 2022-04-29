using UnityEngine;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 3/12/22
 */
namespace PJ
{
    namespace Graph
    {
        /// <summary>
        /// Chooses a type (string). Types can be used to instantiate based on class name, asset name, or identifier
        /// </summary>
        public abstract class SomeTypeChoiceNode : SomeChoiceNode<string>
        {
        }
    }
}
