using System;
using System.Collections.Generic;

/*
 * RATING: 5 stars. Simple design pattern with Unit Tests
 * CODE REVIEW: 4.7.18
 */
namespace PJ {
	public abstract class AbstractBroadcaster {
		public virtual void RemoveListener(Listener listener) {}
	}

	/// <summary>
	/// Broadcaster sends messages to listeners.
	/// </summary>
	public class Broadcaster : AbstractBroadcaster
	{
		public HashSet<WeakReference> listeners { get; protected set; }

		public Broadcaster()
		{
			listeners = new HashSet<WeakReference>();
		}

		public override void RemoveListener(Listener listener)
		{
			foreach (WeakReference wr in listeners)
			{
				if (wr.Target == listener) {
					listeners.Remove(wr);
					break;
				}
			}
		}

		public virtual void AddListener(Listener listener) {
			if (null == listener) {
				return;
			}
			listeners.Add(new WeakReference(listener));
		}

		public void RemoveAllListeners() {
			listeners.Clear();
		}
	
		public void Broadcast(Event theEvent) {
			var deadReferences = new List<WeakReference>(); 

			foreach (WeakReference wr in listeners) {
				if (!wr.IsAlive) {
					deadReferences.Add(wr);
					continue;
				}

				var listener = wr.Target as Listener;
				if (null == listener) {
					continue;
				}

				listener.EvtListen(theEvent);
			}

			foreach (WeakReference wr in deadReferences) {
				listeners.Remove(wr);
			}
		}
	}

}
