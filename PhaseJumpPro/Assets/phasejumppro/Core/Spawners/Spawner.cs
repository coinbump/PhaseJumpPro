using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/15/22
 */
namespace PJ
{
	/// <summary>
	/// Spawn after N seconds
	/// </summary>
    // FUTURE: add unit tests if needed
	public class Spawner : SpawnTable
	{
		public GameObject spawnParent;

		/// <summary>
        /// Timer sends OnFinish to owner.
        /// </summary>
		protected class SpawnFireTimer : Timer
		{
			protected WeakReference<Spawner> owner;

			public SpawnFireTimer(Spawner owner, float duration) : base(duration, SomeTimed.RunType.KeepRunning)
			{
				this.owner = new WeakReference<Spawner>(owner);
			}

			protected override void OnFinish()
			{
				base.OnFinish();

				if (owner.TryGetTarget(out Spawner spawner))
                {
					spawner.OnSpawnTimerFinish();
                }
			}
		}

		/// <summary>
        /// (Optional) action when game object is spawned
        /// </summary>
		public Action<GameObject> spawnAction;

		[SerializeField]
		protected float spawnTime = 0;

		public Timer SpawnTimer
        {
			get
            {
				if (null == spawnTimer)
                {
					spawnTimer = new SpawnFireTimer(this, spawnTime);
				}
				return spawnTimer;
			}
        }

		public float SpawnTime
		{
			get => spawnTime;
			set
			{
				spawnTime = value;
				SpawnTimer.duration = spawnTime;
			}
		}

		protected Timer spawnTimer;

		public override void OnUpdate(TimeSlice time)
		{
			base.OnUpdate(time);
			SpawnTimer.OnUpdate(time);
		}

		public virtual bool CanSpawn()
        {
			return true;
        }

		public virtual GameObject Spawn()
		{
			if (!CanSpawn()) { return null; }

			GameObject template = NextSpawn();
			if (null == template) { return null; }

			GameObject newSpawn = Instantiate(template, Vector3.zero, Quaternion.identity);

			var spawnPosition = SpawnPositionFor(newSpawn);
			//Debug.Log("Spawn Position: " + spawnPosition.ToString());

			newSpawn.transform.parent = transform;
			newSpawn.transform.localPosition = spawnPosition;

			if (null == spawnParent)
			{
				newSpawn.transform.parent = null;
			}
			else
            {
				newSpawn.transform.parent = spawnParent.transform;
            }

			OnSpawn(newSpawn);

			if (null != spawnAction)
            {
				spawnAction(newSpawn);
			}

			return newSpawn;
		}

		public void OnSpawnTimerFinish()
        {
			Spawn();
		}

		/// <summary>
        /// Spawn position for new spawn
        /// </summary>
        /// <param name="gameObject"></param>
        /// <returns>Returns local position to spawn</returns>
		protected virtual Vector3 SpawnPositionFor(GameObject gameObject) { return Vector3.zero; }

		protected virtual void OnSpawn(GameObject gameObject)
		{
		}
	}
}
