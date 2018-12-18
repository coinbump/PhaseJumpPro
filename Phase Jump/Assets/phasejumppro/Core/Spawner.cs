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
	/// Spawn after N seconds
	/// </summary>
	/// TODO: could use a unit test
	class Spawner : SpawnTable
	{
		public float timer = 0; // If > 0, spawn after N seconds

		private Timer spawnTimer = new Timer(AbstractTimed.Type.Persistent);

		protected override void Start()
		{
			spawnTimer.duration = timer;
		}

		protected virtual void Update()
		{
			if (spawnTimer.IsRunning && spawnTimer.duration > 0)
			{
				spawnTimer.EvtUpdate(new TimeSlice(Time.deltaTime));
				if (spawnTimer.IsFinished)
				{
					GameObject spawn = NewSpawn();
					Instantiate(spawn, transform.position, Quaternion.identity);

					spawnTimer.Reset();
				}
			}
		}
	}
}
