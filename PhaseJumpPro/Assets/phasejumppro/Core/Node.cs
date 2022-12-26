using System;
using UnityEngine;

/*
RATING: 5 stars
Simple component
CODE REVIEW: 12/18/22
*/
namespace PJ
{
    /// <summary>
    /// Base 3D world node
    /// </summary>
    public class Node : SomeNode
    {
        protected new Rigidbody rigidbody;

        public override bool IsKinematic
        {
            get => null == rigidbody ? true : rigidbody.isKinematic;
        }

        protected override void Awake()
        {
            base.Awake();

            rigidbody = GetComponent<Rigidbody>();
        }

        /// <summary>
        /// Move this node to the position in 3D space
        /// </summary>
        public override void MoveToPosition(Vector3 position, bool force = false)
        {
            if (IsKinematic || force)
            {
                transform.position = position;
            }
            else
            {
                rigidbody.MovePosition(position);
            }
        }
    }
}
