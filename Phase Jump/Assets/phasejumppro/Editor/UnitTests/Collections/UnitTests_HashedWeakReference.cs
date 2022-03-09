using UnityEngine;
using System;
using System.Collections.Generic;
using NUnit.Framework;

namespace PJ
{
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
		}
	}
}
