﻿using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 12/29/22
 */
namespace PJ
{
    /// <summary>
    /// Move the object in one direction. When it collides, bounce
    /// </summary>
    public class BounceSteering2D : SomeSteering2D
    {
        protected GoNode2D node;

        protected override void Start()
        {
            base.Start();

            node = GetComponent<GoNode2D>();
            if (null == node)
            {
                Debug.Log("Error. Bounce steering requires node");
                return;
            }
        }

        public void OnCollisionEnter2D(Collision2D collision)
        {
            if (null == node) { return; }

            switch (node.VelocityType)
            {
                case GoNode2D.MoveType.Vector:
                    break;
                default:
                    Debug.Log("Error. Bounce steering requires vector velocity");
                    return;
            }

            var contacts = new List<ContactPoint2D>();

            collision.GetContacts(contacts);
            foreach (ContactPoint2D contact in contacts)
            {
                var velocity = node.Velocity;
                node.Velocity = Vector2.Reflect(velocity, contact.normal);
            }
        }
    }
}
