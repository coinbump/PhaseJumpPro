using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 11/20/22
*/
namespace PJ
{
    /// <summary>
    /// Destroys objects that collide with it
    /// Can be placed outside screen bounds to get rid of objects that move offscren
    /// </summary>
    public class Culler2D : MonoBehaviour
    {
        /// <summary>
        /// Restricts culling to nodes with these type tags
        /// </summary>
        public List<String> cullTags = new List<String>();

        protected virtual bool ShouldCull(GameObject gameObject)
        {
            if (cullTags.Count == 0) { return true; }

            var node = gameObject.GetComponent<SomeNode>();
            if (node) {
                foreach (var tag in cullTags) {
                    if (node.HasTypeTag(tag)) {
                        return true;
                    }
                }
            }

            return false;
        }

        protected virtual void OnCollisionEnter2D(Collision2D collision) {
            if (!ShouldCull(collision.gameObject)) { return; }
            Destroy(collision.gameObject);
        }

        protected virtual void OnTriggerEnter2D(Collider2D collider) {
            if (!ShouldCull(collider.gameObject)) { return; }
            Destroy(collider.gameObject);
        }
    }
}
