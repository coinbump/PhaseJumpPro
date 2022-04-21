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
        public abstract class SomeChoiceNode<Type> : GoStandardAcyclicNode
        {
            public abstract HashSet<Type> Choices { get; }
            public abstract Type Choose();
        }
    }
}
