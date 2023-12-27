﻿using System;
using System.Collections;
using System.Collections.Generic;
using Godot;

/*
 * RATING: 5 stars.
 * Has unit tests
 * CODE REVIEW: 12/31/22
 */
namespace PJ
{
    /// <summary>
    /// Spawn (or loot) tables specify the weighted probability of spawning an object
    /// EXAMPLE: leave item behind when enemy is defeated (or nothing)
    /// </summary>
    // ALSO KNOWN AS: Loot table
    [Serializable]
    public class SpawnTable
    {
        [Serializable]
        public class Item
        {
            [Tooltip("Object to be spawned (or null for empty)")]
            public Node spawnObject;

            [Tooltip("Weighted value for spawning this element.")]
            public float weight = 1.0f;

            public Item()
            {
            }

            public Item(Node spawnObject, float weight)
            {
                this.spawnObject = spawnObject;
                this.weight = weight;
            }
        }

        public List<Item> spawnItems = new();

        public SomeRandom random = new EngineRandom();

        /// <summary>
        /// Spawn the next object from the spawn table
        /// If all objects have weight 0, will return null
        /// </summary>
        public Node NextSpawn()
        {
            var wr = new WeightedRandomChoice<Item>();

            float totalWeight = 0.0f;

            foreach (Item item in spawnItems)
            {
                totalWeight += item.weight;
                wr.Add(new Weight<Item>(item.weight, item));
            }

            var weight = wr.ChooseWeight(random);
            if (null == weight) { return null; }
            if (null == weight.value) { return null; }

            return weight.value.spawnObject;
        }
    }
}
