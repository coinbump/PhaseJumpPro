using System;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/13/22
 */
namespace PJ
{
    public interface SomeBroadcaster
    {
        public void AddListener(SomeListener listener);
        public void RemoveListener(SomeListener listener);
    }

    /// <summary>
    /// Broadcaster sends messages to listeners.
    /// </summary>
    public class Broadcaster : SomeBroadcaster
    {
        public HashSet<HashedWeakReference<SomeListener>> listeners { get; protected set; }

        public Broadcaster()
        {
            listeners = new HashSet<HashedWeakReference<SomeListener>>();
        }

        public void RemoveListener(SomeListener listener)
        {
            foreach (HashedWeakReference<SomeListener> reference in listeners)
            {
                if (reference.Reference.TryGetTarget(out SomeListener iterListener) && iterListener == listener)
                {
                    listeners.Remove(reference);
                    break;
                }
            }
        }

        public virtual void AddListener(SomeListener listener)
        {
            if (null == listener)
            {
                return;
            }
            listeners.Add(new HashedWeakReference<SomeListener>(listener));
        }

        public void Clear()
        {
            listeners.Clear();
        }

        public void Broadcast(Event theEvent)
        {
            // Prune garbage collected listeners
            var newListeners = new HashSet<HashedWeakReference<SomeListener>>();

            foreach (HashedWeakReference<SomeListener> reference in listeners)
            {
                if (reference.Reference.TryGetTarget(out SomeListener listener))
                {
                    listener.OnEvent(theEvent);
                    newListeners.Add(new HashedWeakReference<SomeListener>(listener));
                }
            }

            listeners = newListeners;
        }
    }
}
