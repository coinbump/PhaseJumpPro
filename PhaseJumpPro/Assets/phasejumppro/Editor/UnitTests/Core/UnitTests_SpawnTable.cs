using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using NUnit.Framework;
using System;

namespace PJ
{
	public class UnitTests_SpawnTable
	{
		[Test]
		public void TestSpawnTable()
		{
			var gameObject1 = new GameObject();
			gameObject1.name = "1";

			var gameObject2 = new GameObject();
			gameObject2.name = "2";

			var fixedRandom = new FixedRandom();
			fixedRandom.values.Add(.2f);
			fixedRandom.values.Add(.7f);

			var spawnTable = gameObject1.AddComponent<SpawnTable>();
			spawnTable.random = fixedRandom;
			spawnTable.spawnItems.Add(new SpawnTable.Item(gameObject1, 0.5f));
			spawnTable.spawnItems.Add(new SpawnTable.Item(gameObject2, 0.5f));

			// FUTURE: add more unit tests if needed
			var nextSpawn = spawnTable.NextSpawn();
            Assert.AreEqual("1", nextSpawn.name);

			nextSpawn = spawnTable.NextSpawn();
			Assert.AreEqual("2", nextSpawn.name);

			GameObject.DestroyImmediate(gameObject1);
		}
	}
}
