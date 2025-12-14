using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Simple abstract
CODE REVIEW: 1/19/23
*/
namespace PJ
{
    public enum CardPlayMode
    {
        // Card can be played directly
        Play,

        // card requires a target to play
        Target
    }

    /// <summary>
    /// For every battle action (heal, attack, buff), there will be a corresponding animation
    /// We don't apply model changes directly. Instead we create a series of action that
    /// can be converted to animations as needed
    /// </summary>
    public abstract class SomeBattleAction : Core
    {
    }

    /// <summary>
    /// A card that can target other entities
    /// </summary>
    public abstract class SomeBattleCard<TargetType, BattleModel> : SomeCard
    {
        /// <summary>
        /// (OPTIONAL) Cost overrides to play the card on turn (removed after turn)
        /// </summary>
        public Dictionary<string, int> turnCosts = new();

        /// <summary>
        /// Determines the targeting mode for this card
        /// </summary>
        public virtual CardPlayMode PlayMode() { return CardPlayMode.Play; }

        public virtual bool IsValidTarget(TargetType target) { return false; }
        public virtual List<SomeBattleAction> Target(BattleModel battleModel, TargetType target) { return new(); }
        public virtual List<SomeBattleAction> Play(BattleModel battleModel) { return new(); }

        /// <summary>
        /// Returns true if the card is playable for the battle state
        /// </summary>
        public virtual bool IsPlayable(BattleModel battleModel) { return true; }
    }
}
