using UnityEngine;
using System;
using System.Collections.Generic;
using NUnit.Framework;

namespace PJ
{
	// FUTURE: Support GC.Collect tests, but there are some limitations:
	// https://stackoverflow.com/questions/15205891/garbage-collection-should-have-removed-object-but-weakreference-isalive-still-re
	public class UnitTests_HashedWeakReference
	{
		private class Foo
        {

        }

		/// <summary>
        /// Test expected behavior for WeakReference
        /// </summary>
		[Test]
		public void TestWeakReferenceInHashSet()
		{
			var foo = new Foo();
			var fooWeak1 = new WeakReference<Foo>(foo);
			var fooWeak2 = new WeakReference<Foo>(foo);

			Assert.AreNotEqual(fooWeak1, fooWeak2);
			Assert.AreNotEqual(fooWeak1.GetHashCode(), fooWeak2.GetHashCode());
			Assert.IsFalse(fooWeak1.Equals(fooWeak2));

			var set = new HashSet<WeakReference<Foo>>();
			set.Add(fooWeak1);
			set.Add(fooWeak2);

			Assert.AreEqual(2, set.Count);
		}

		[Test]
		public void TestHashedWeakReferenceInHashSet()
		{
			var foo = new Foo();
			var fooWeak1 = new HashedWeakReference<Foo>(foo);
			var fooWeak2 = new HashedWeakReference<Foo>(foo);

			Assert.AreEqual(fooWeak1, fooWeak2);
			Assert.AreEqual(fooWeak1.GetHashCode(), fooWeak2.GetHashCode());
			Assert.IsTrue(fooWeak1.Equals(fooWeak2));

			var set = new HashSet<HashedWeakReference<Foo>>();
			set.Add(fooWeak1);
			set.Add(fooWeak2);

			Assert.AreEqual(1, set.Count);
		}
	}
}
