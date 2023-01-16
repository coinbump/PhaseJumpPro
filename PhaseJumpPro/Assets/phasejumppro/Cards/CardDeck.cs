using System;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// A deck of cards
    /// </summary>
    public class CardDeck<CardType> : Core where CardType : SomeCard
    {
        public List<CardType> cards = new();

        CardDeckClass<CardType> CardDeckClass => _class as CardDeckClass<CardType>;

        public CardDeck(CardDeckClass<CardType> _class)
        {
            this._class = _class;
        }

        /// <summary>
        /// Add 1 of each card type to the deck
        /// </summary>
        public virtual void Build()
        {
            cards.Clear();

            var cardDeckClass = CardDeckClass;
            if (null == cardDeckClass)
            {
                Debug.Log("Error. CardDeck requires a CardDeckClass");
                return;
            }

            foreach (var classCard in cardDeckClass.cards)
            {
                Add(classCard.id, 1, classCard.tags);
            }
        }

        public virtual void Shuffle() => cards.Shuffle();

        /// <summary>
        /// Add cards based on id (requires CardDeckClass) and apply custom tag attributes
        /// </summary>
        public List<CardType> Add(string id, int count, Tags tags)
        {
            List<CardType> result = new();

            var cardDeckClass = CardDeckClass;
            if (null == cardDeckClass) { return result; }

            for (int i = 0; i < count; i++)
            {
                var card = cardDeckClass.New(id);
                if (null == card) { return result; }

                card.id = id;

                if (null != tags)
                {
                    card.ApplyTags(tags);
                }

                cards.Add(card);
                result.Add(card);
            }

            return result;
        }

        public virtual void Clear()
        {
            cards.Clear();
        }

        // Convenience names
        public void RemoveAll() { Clear(); }
    }
}
