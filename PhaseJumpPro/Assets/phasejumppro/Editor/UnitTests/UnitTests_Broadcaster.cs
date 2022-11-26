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
			var sut = new Broadcaster();
			var listener = new TestListener();
			var listener2 = new TestListener();
			sut.AddListener(listener);
			Assert.AreEqual(1, sut.listeners.Count);
			sut.RemoveListener(listener);
			Assert.AreEqual(0, sut.listeners.Count);

			sut.AddListener(listener);
			sut.AddListener(listener2);
			sut.Broadcast(new Event("hello"));
			Assert.AreEqual("hello", listener.lastMessage);
			Assert.AreEqual("hello", listener2.lastMessage);

			sut.RemoveListener(listener);
			sut.Broadcast(new Event("goodbye"));
			Assert.AreEqual("hello", listener.lastMessage);
			Assert.AreEqual("goodbye", listener2.lastMessage);

			sut.Clear();
			Assert.AreEqual(0, sut.listeners.Count);
		}
	}
}
