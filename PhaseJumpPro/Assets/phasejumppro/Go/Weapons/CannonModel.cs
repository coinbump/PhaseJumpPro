using System;
using UnityEngine;
using System.Collections.Generic;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/31/22
*/
// FUTURE: support hitscan weapons, piercing hitscan (goes through multiple targets)
// FUTURE: support charge-type weapons
// FUTURE: support chain-weapons (electricity): chain, loses some power as it chains
// FUTURE: support grenade-type weapons, laser-type weapons
namespace PJ
{
    public class CannonModel : Updatable
    {
        /// <summary>
        /// Min time between emits (decrease for rapid fire)
        /// </summary>
        public float limitTime;

        public StandardEmitsModel emitsModel = new();

        public bool IsFinished { get => false; }

        public string id;

        protected RateLimiter limiter = new RateLimiter(0);
        protected float velocityMagnitude;

        public float VelocityMagnitude
        {
            set
            {
                this.velocityMagnitude = value;
                emitsModel.VelocityMagnitude = value;
            }
        }

        public bool Fire() => limiter.Fire();

        public void OnUpdate(TimeSlice time)
        {
            limiter.minDelta = limitTime;
            limiter.OnUpdate(time);
        }

        public void AddBurstFire(int count, float interval)
        {
            var iterModels = new List<StandardEmitsModel.EmitModel>(emitsModel.models);

            for (int c = 0; c < count; c++)
            {
                for (int i = 0; i < iterModels.Count; i++)
                {
                    var model = iterModels[i];

                    var newModel = new StandardEmitsModel.EmitModel();
                    newModel.velocity = model.velocity;
                    newModel.offset = model.offset;
                    newModel.velocityTransform = model.velocityTransform;
                    newModel.delay = (c + 1) * interval;
                    newModel.lifeTime = model.lifeTime;

                    emitsModel.models.Add(newModel);
                }
            }
        }
    }
}
