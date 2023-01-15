using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Bullet nodes have some common behavior in all games
    /// </summary>
    public class Bullet2D : GoNode2D
    {
        [Space(10)]
        [Header("Bullet2D Values")]
        public float damageValue = 0;

        public override void OnTransformLimited()
        {
            //Debug.Log("Bullet Transform limited");

            // Bullet is destroyed when it hits a wall (for Kinematic movement)
            Destroy(gameObject);
        }
    }
}
