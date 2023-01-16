using System;
using System.Collections.Generic;
using Newtonsoft.Json;
using UnityEngine;

/*
RATING: 5 stars
Core utility
CODE REVIEW: 1/15/23
*/
namespace PJ
{
    /// <summary>
    /// Un-typed CardDeckClass
    /// </summary>
    public class SomeCardDeckClass : Class
    {
        public class Card
        {
            public string id;
            public Tags tags = new();
        }

        public List<Card> cards = new();

        public SomeCardDeckClass(string id) : base(id)
        {
        }
    }

    /// <summary>
    /// CardDeckClass with card type
    /// </summary>
    public class CardDeckClass<CardType> : SomeCardDeckClass where CardType : SomeCard
    {
        /// <summary>
        /// Registry of factories to create cards, each with a unique ID
        /// </summary>
        public FactoryRegistry<CardType> cardRegistry = new();

        public CardDeckClass(string id) : base(id)
        {
        }

        public virtual CardType New(string id) => cardRegistry.New(id);
    }
}
