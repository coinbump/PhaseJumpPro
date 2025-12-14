using Godot;
using System.Collections;
using System.Collections.Generic;
using System;

/*
 * RATING: 5 stars
 * Utility class
 * CODE REVIEW: 4/18/22
 */
namespace PJ
{
    /// <summary>
    /// Keeps track of objects that are alive without keeping strong references
    /// For emitters/spawners that need to keep track of the objects they've spawned
    /// </summary>
    public class WeakNodeList : List<WeakReference<Node>>
    {
        public void Add(Node node)
        {
            Add(new WeakReference<Node>(node));
        }

        public void Remove(Node node)
        {
            Remove(new WeakReference<Node>(node));
        }

        /// <summary>
        /// Call this to get rid of old/dead objects
        /// </summary>
        public void Refresh()
        {
            var removeEmits = new List<WeakReference<Node>>();
            foreach (WeakReference<Node> r in this)
            {
                // A) Game object is marked "dead", 
                // B) Accessing its name produces an exception (the object was destroyed, but we still have a reference to it)
                // C) The WeakReference target is gone (Garbage Collector caught up)
                if (r.TryGetTarget(out Node node))
                {
                    try
                    {
                        if (node.Name == "_dead_")
                        {
                            removeEmits.Add(r);
                        }
                    }
                    catch
                    {
                        removeEmits.Add(r);
                    }
                }
                else
                {
                    removeEmits.Add(r);
                }
            }

            foreach (WeakReference<Node> r in removeEmits)
            {
                Remove(r);
            }
        }
    }
}
