using System;
using UnityEngine;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Value type
 * CODE REVIEW: 4/14/22
 */
namespace PJ
{
    public class Weapon2D : Weapon
    {
        public List<float> emitAngles = new List<float>();
        public List<float> varyEmitAngles = new List<float>();

        /// <summary>
        /// Emit N bullets, spread by N degrees each, centered to 0
        /// </summary>
        public void EmitSpread(int numBullets, float degreeAngleStep, float varyAngle = 0)
        {
            if (0 == numBullets) { return; }

            emitCount = numBullets;
            emitAngles.Clear();

            var firstAngle = -degreeAngleStep * ((float)(numBullets - 1) / 2);

            for (int i = 0; i < numBullets; i++)
            {
                emitAngles.Add(firstAngle + i * degreeAngleStep);
                varyEmitAngles.Add(varyAngle);
            }
        }
    }
}
