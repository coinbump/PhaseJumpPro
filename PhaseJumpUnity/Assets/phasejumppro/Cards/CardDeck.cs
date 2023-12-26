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
            this.Class = _class;
        }

        /// <summary>
        /// Add 1 of each card type to the deck
        /// </summary>
        public virtual void Build(Func<CardType, bool> filter, int count = 1)
        {
            cards = BuildCards(filter, count);
        }

        public virtual List<CardType> BuildCards(Func<CardType, bool> filter, int count = 1)
        {
            List<CardType> result = new();

            var cardDeckClass = CardDeckClass;
            if (null == cardDeckClass)
            {
                Debug.Log("Error. CardDeck requires a CardDeckClass");
                return result;
            }

            foreach (var classCard in CardDeckClass.cardRegistry)
            {
                var card = CardDeckClass.New(classCard.Key);
                if (filter(card))
                {
                    AddTo(result, card.ClassId, count);
                }
            }

            return result;
        }

        public virtual void BuildWeighted(int count, Func<CardType, bool> filter, Func<CardType, float> weightTransform, SomeRandom random)
        {
            cards = BuildWeightedCards(count, filter, weightTransform, random);
        }

        public virtual List<CardType> BuildWeightedCards(int count, Func<CardType, bool> filter, Func<CardType, float> weightTransform, SomeRandom random)
        {
            List<CardType> result = new();

            var allCards = BuildCards(filter);
            var wr = new WeightedRandomChoice<CardType>();
            foreach (var card in allCards)
            {
                var weight = weightTransform(card);
                if (weight == 0) { continue; }
                wr.Add(new(weight, card));
            }

            for (int i = 0; i < count; i++)
            {
                var choice = wr.ChooseWeight(random);
                AddTo(result, choice.value.ClassId, 1);
            }

            return result;
        }

        public virtual void Shuffle(SomeRandom random) => cards.Shuffle(random);

        /// <summary>
        /// Add cards based on id (requires CardDeckClass) and apply custom tag attributes
        /// </summary>
        public List<CardType> Add(string classId, int count)
        {
            return AddTo(cards, classId, count);
        }

        public List<CardType> AddTo(List<CardType> cards, string classId, int count)
        {
            List<CardType> result = new();

            var cardDeckClass = CardDeckClass;
            if (null == cardDeckClass) { return result; }

            for (int i = 0; i < count; i++)
            {
                var card = CardDeckClass.New(classId);
                if (null == card) { return result; }

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
