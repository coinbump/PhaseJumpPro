using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using NUnit.Framework;
using System;

namespace PJ
{
    public class UnitTests_Spawner
    {
        protected List<GameObject> spawnList = new List<GameObject>();

        [Test]
        public void TestSpawner()
        {
            var gameObject1 = new GameObject();
            gameObject1.name = "1";

            var gameObject2 = new GameObject();
            gameObject2.name = "2";

            var fixedRandom = new FixedNormalRandom();
            fixedRandom.values.Add(.2f);
            fixedRandom.values.Add(.7f);

            var spawner = gameObject1.AddComponent<Spawner>();
            spawner.spawnAction = AddSpawn;

            spawner.random = fixedRandom;
            spawner.spawnItems.Clear();
            spawner.spawnItems.Add(new SpawnTable.Item(gameObject1, 0.5f));
            spawner.spawnItems.Add(new SpawnTable.Item(gameObject2, 0.5f));

            spawner.SpawnTime = 1.0f;
            spawner.OnUpdate(new TimeSlice(0.5f));
            Assert.AreEqual(0, spawnList.Count);

            spawner.OnUpdate(new TimeSlice(1.0f));
            Assert.AreEqual(1, spawnList.Count);

            spawner.OnUpdate(new TimeSlice(.9f));
            Assert.AreEqual(1, spawnList.Count);

            spawner.OnUpdate(new TimeSlice(.3f));
            Assert.AreEqual(2, spawnList.Count);

            // Unity appends "(Clone)" to objects, so check the first character
            Assert.AreEqual('1', spawnList[0].name[0]);
            Assert.AreEqual('2', spawnList[1].name[0]);
        }

        protected void AddSpawn(GameObject spawn)
        {
            spawnList.Add(spawn);
        }
    }
}
