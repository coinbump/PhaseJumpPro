using System;
using UnityEngine;

namespace PJ
{
    public class Node : SomeNode
    {
        protected new Rigidbody rigidbody;

        public override bool IsKinematic
        {
            get
            {
                if (null == rigidbody)
                {
                    return false;
                }
                return rigidbody.isKinematic;
            }
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
                //Debug.Log("Move to Position: Kinematic");

                transform.position = position;
            }
            else
            {
                if (null != rigidbody)
                {
                    //Debug.Log("Move to Position: Physics");

                    // MovePosition is for physics-based objects (non kinematic)
                    rigidbody.MovePosition(position);
                }
            }
        }
    }
}
