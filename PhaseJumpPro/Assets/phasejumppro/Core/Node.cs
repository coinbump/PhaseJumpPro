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
    }
}
