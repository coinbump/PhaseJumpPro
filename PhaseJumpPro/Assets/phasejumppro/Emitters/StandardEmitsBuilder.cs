using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Simple utility
CODE REVIEW: 12/31/22
*/
namespace PJ
{
    /// <summary>
    /// Uses standard emits model to build emits
    /// </summary>
    public class StandardEmitsBuilder : SomeEmitsBuilder
    {
        public StandardEmitsModel model = new();

        public StandardEmitsBuilder(StandardEmitsModel model)
        {
            this.model = model;
        }

        public override List<Emit> BuildEmits()
        {
            List<Emit> result = new();

            for (int i = 0; i < model.models.Count; i++)
            {
                var emitModel = model.models[i];
                var emitVector = emitModel.velocity;

                if (null != emitModel.velocityTransform)
                {
                    emitVector = emitModel.velocityTransform.Transform(emitVector);
                }

                result.Add(new(emitVector, emitModel.offset, emitModel.delay, emitModel.lifeTime));
            }

            return result;
        }

        public void Clear()
        {
            model.Clear();
        }
    }
}
