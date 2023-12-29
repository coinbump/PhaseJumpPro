using System;
using Godot;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Value type
 * CODE REVIEW: 12/31/22
 */
namespace PJ
{
    /// <summary>
    /// Model for a melee weapon
    /// </summary>
    public class MeleeWeaponModel : Updatable
    {
        public bool IsFinished => false;

        public string id;
        public Node weaponObject;

        /// <summary>
        /// Min time between emits (decrease for rapid fire)
        /// </summary>
        public float limitTime;

        protected RateLimiter limiter = new RateLimiter(0);

        public bool Fire() => limiter.Fire();

        public void OnUpdate(TimeSlice time)
        {
            limiter.minDelta = limitTime;
            limiter.OnUpdate(time);
        }
    }
}
