using System;
using UnityEngine;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Value type
 * CODE REVIEW: 4/13/22
 */
// FUTURE: support hitscan weapons, piercing hitscan (goes through multiple targets)
// FUTURE: support charge-type weapons
// FUTURE: support chain-weapons (electricity): chain, loses some power as it chains
// FUTURE: support grenade-type weapons, laser-type weapons
namespace PJ
{
    public class Weapon : Updatable {
        public enum WeaponType
        {
            Bullet,

            // Melee is a child object of the emitter so it moves with them
            Melee
        }

        public enum FireType
        {
            // Fire once per tap (if limiter allows it)
            Once,

            // Fire N times per tap
            Burst,

            // Automatic fire while button is held down
            Auto
        }

        public string id;
        public WeaponType type = WeaponType.Bullet;
        public FireType fireType = FireType.Once;

        public GameObject bulletObject;

        public float velocity;
        public float varyVelocity;

        /// <summary>
        /// Min time between emits (decrease for rapid fire)
        /// </summary>
        public float limitTime;

        /// <summary>
        /// Life time before bullet is destroyed (if > 0)
        /// </summary>
        public float lifeTime;

        /// <summary>
        /// Emit count per fire
        /// </summary>
        public int emitCount = 1;

        /// <summary>
        /// Number of fires for burst fire mode
        /// </summary>
        public int burstCount = 2;

        /// <summary>
        /// Interval between each burst bullet
        /// </summary>
        public float burstInterval = .1f;

        /// <summary>
        /// Damage that a bullet does
        /// </summary>
        public float bulletDamage = 1;

        protected RateLimiter limiter = new RateLimiter(0);

        public bool Fire() => limiter.Fire();

        public void OnUpdate(TimeSlice time)
        {
            limiter.minDelta = limitTime;
            limiter.OnUpdate(time);
        }
    }
}
