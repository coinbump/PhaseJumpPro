using UnityEngine;
using System;

namespace PJ
{
    public interface SomeCollider
    {
        bool Enabled { get; set; }
    }

    /// <summary>
    /// Allows us to write code for either 2D or 3D colliders, without needing to
    /// know which type of collider the object is using
    /// </summary>
    public class MultiCollider : SomeCollider
    {
        protected Collider collider;
        protected Collider2D collider2D;

        public MultiCollider(GameObject gameObject)
        {
            collider = gameObject.GetComponent<Collider>();
            collider2D = gameObject.GetComponent<Collider2D>();
        }

        public bool Enabled
        {
            get
            {
                if (null != collider)
                {
                    return collider.enabled;
                }
                else if (null != collider2D)
                {
                    return collider2D.enabled;
                }

                return false;
            }
            set
            {
                if (null != collider)
                {
                    collider.enabled = value;
                }
                else if (null != collider2D)
                {
                    collider2D.enabled = value;
                }
            }
        }
    }
}
