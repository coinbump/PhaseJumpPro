using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars.
 * Has unit tests
 * CODE REVIEW: 4/15/22
 */
namespace PJ
{
	/// <summary>
	/// Spawn (or loot) tables specify the weighted probability of spawning an object
	/// EXAMPLE: leave item behind when enemy is defeated (or nothing)
	/// </summary>
    // ALSO KNOWN AS: Loot table
	public class SpawnTable : MonoBehaviour
	{
		[Serializable]
		public class Item
		{
			[Tooltip("Object to be spawned (or null for empty)")]
			public GameObject spawnObject;

			[Tooltip("Weighted value for spawning this element.")]
			public float weight = 1.0f;

			public Item()
            {
            }

			public Item(GameObject spawnObject, float weight)
            {
				this.spawnObject = spawnObject;
				this.weight = weight;
            }
		}

		[Tooltip("Items in the spawn table.")]
		public List<Item> spawnItems = new List<Item>() { new Item() };

		public SomeRandom random;

		public GameObject NextSpawn()
		{
			if (null == random)
            {
				random = new UnityRandom();
			}
			WeightedRandom wr = new WeightedRandom(random);

			float totalWeight = 0.0f;

			foreach (Item item in spawnItems)
			{
				totalWeight += item.weight;
				wr.Add(new Weight(item.weight, item));
			}

			var weight = wr.ChooseRandom();
			if (null == weight) { return null; }
			if (null == weight.target) { return null; }

			return ((Item)weight.target).spawnObject;
		}

		protected virtual void Start()
		{
		}
	}
}
