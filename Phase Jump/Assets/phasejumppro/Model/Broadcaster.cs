using System;
using System.Collections.Generic;
using NUnit.Framework;

/*
 * RATING: 5 stars. Simple design pattern with Unit Tests
 * CODE REVIEW: 4.7.18
 */
namespace PJ {
	public abstract class AbstractBroadcaster {
		virtual public void RemoveListener(Listener listener) {}
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

		private class TestListener : Listener {
			public int listenCount;
			public string lastMessage;

			public override void EvtListen(Event theEvent) {
				base.EvtListen(theEvent);

				listenCount++;
				lastMessage = theEvent.name;
			}
		}

		[Test]
		public void UnitTests()
		{
			var test = new Broadcaster();
			var listener = new TestListener();
			var listener2 = new TestListener();
			test.AddListener(listener);
			Assert.AreEqual(1, test.listeners.Count);
			test.RemoveListener(listener);
			Assert.AreEqual(0, test.listeners.Count);

			test.AddListener(listener);
			test.AddListener(listener2);
			test.Broadcast(new Event("hello"));
			Assert.AreEqual("hello", listener.lastMessage);
			Assert.AreEqual("hello", listener2.lastMessage);

			test.RemoveListener(listener);
			test.Broadcast(new Event("goodbye"));
			Assert.AreEqual("hello", listener.lastMessage);
			Assert.AreEqual("goodbye", listener2.lastMessage);

			test.RemoveAllListeners();
			Assert.AreEqual(0, test.listeners.Count);
		}
	}

}
