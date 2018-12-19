using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 4 stars.
 * Solid, could use a unit test.
 * CODE REVIEW: 12/17/18
 */
namespace PJ
{
	/// <summary>
	/// Spawn (or loot) tables specify the weighted probability of spawning an object.
	/// EXAMPLE: leave item behind when enemy is defeated (or nothing).
	/// 
	/// If you want consistent behavior, make sure that all weights add up to a standard value (1 or 100)
	/// </summary>
	/// TODO: could use a unit test
	public class SpawnTable : MonoBehaviour
	{
		[Serializable]
		public struct Element
		{
			public float weight;
			public GameObject spawn;
		}
		public List<Element> elements = new List<Element>();
		public bool autoNone = true;   // If true, add a none element that results in no result.
		public float maxWeight = 1.0f;

		public GameObject NextSpawn()
		{
			WeightedRandom wr = new WeightedRandom();

			float totalWeight = 0.0f;

			foreach (Element element in elements)
			{
				totalWeight += element.weight;
				wr.Add(new Weight(element.weight, element));
			}

			if (autoNone && totalWeight < maxWeight)
			{
				wr.Add(new Weight(maxWeight - totalWeight, null));
			}

			var weight = wr.ChooseRandom();
			if (null != weight.theObject)
			{
				return ((PJ.SpawnTable.Element)weight.theObject).spawn;
			}

			return null;
		}

		protected virtual void Start()
		{
		}
	}
}
