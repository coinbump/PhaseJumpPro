using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Simple abstract
CODE REVIEW: 1/15/23
*/
namespace PJ
{
    /// <summary>
    /// A card in a deck of cards
    /// </summary>
    public class SomeCard : Core
    {
        public string id;

        public virtual void ApplyTags(Tags tags)
        {
            this.tags = tags;
        }
    }
}
