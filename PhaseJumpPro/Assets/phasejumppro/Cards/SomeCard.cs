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

        /// <summary>
        /// (OPTIONAL) Rarity of the card (higher is more rare)
        /// </summary>
        public int rarity;

        /// <summary>
        /// Type tags to categorize the card ("attack", "defend", "retain")
        /// </summary>
        public TypeTagsSet typeTags = new();

        /// <summary>
        /// (OPTIONAL) Costs to play the card
        /// </summary>
        public Dictionary<string, int> costs = new();

        public virtual void ApplyTags(Tags tags)
        {
            this.tags = tags;
        }
    }
}
