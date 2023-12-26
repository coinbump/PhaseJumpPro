using System.Collections.Generic;
using UnityEngine;
using System;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/31/22
*/
namespace PJ
{
    /// <summary>
    /// Emits when Fire is called. The model can define a limiter that limits
    /// How often the cannon can fire
    /// </summary>
    public abstract class SomeCannonEmitter : Emitter
    {
        protected CannonModel model = new();

        public CannonModel Model
        {
            set
            {
                this.model = value;
            }
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            // Update for fire timer
            model.OnUpdate(time);
        }

        public override List<GameObject> Emit()
        {
            var emitsBuilder = new StandardEmitsBuilder(model.emitsModel);
            return EmitWithBuilder(emitsBuilder);
        }

        protected override void OnEmitTimerFinish()
        {
            // Use Fire, not timers
        }
    }
}
