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
	public class Spawner : SpawnTable
	{
		[SerializeField]
		protected float timer = 0; // If > 0, spawn after N seconds

		public float Timer
		{
			get => timer;
			set
			{
				timer = value;
				spawnTimer.duration = timer;
			}
		}

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
					Spawn();

					spawnTimer.Reset();
				}
			}
		}

		public virtual void Spawn()
		{
			GameObject spawn = NextSpawn();
			GameObject newSpawn = Instantiate(spawn, GetSpawnPosition(), Quaternion.identity);
			SetupSpawn(newSpawn);
		}

		protected virtual Vector3 GetSpawnPosition() { return transform.position; }

		protected virtual void SetupSpawn(GameObject go)
		{
		}
	}
}
