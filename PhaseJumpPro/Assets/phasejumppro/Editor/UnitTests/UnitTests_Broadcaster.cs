using System;
using System.Collections.Generic;
using NUnit.Framework;

namespace PJ {
	public abstract class SomeBroadcaster {
		public virtual void RemoveListener(SomeListener listener) {}
	}

	/// <summary>
	/// Broadcaster sends messages to listeners.
	/// </summary>
	public class UnitTests_Broadcaster
	{
		private class TestListener : SomeListener {
			public int listenCount;
			public string lastMessage;

			public void OnListen(Event theEvent) {
				listenCount++;
				lastMessage = theEvent.id;
			}
		}

		[Test]
		public void TestBroadcaster()
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

			test.Clear();
			Assert.AreEqual(0, test.listeners.Count);
		}
	}
}
