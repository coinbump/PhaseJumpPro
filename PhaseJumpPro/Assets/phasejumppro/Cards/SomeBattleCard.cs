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
    /// A card that can target other entities
    /// </summary>
    public abstract class SomeBattleCard<TargetType> : SomeCard
    {
        public abstract void IsValidTarget(TargetType target);
        public abstract void Target(TargetType target);
        public abstract void Play();
    }
}
