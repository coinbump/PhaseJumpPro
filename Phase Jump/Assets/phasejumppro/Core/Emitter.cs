using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ {
	/*
	 * RATING: 4 stars. Simple utility.
	 * TODO: Could use a unit test.
	 * 
	 * CODE REVIEW: 12/18/18
	 */
	public class Emitter : SpawnTable
	{
		enum State
		{
			Emit,   // Emitting
			Cooldown    // Cooldown until we can emit again
		}
		GenericStateMachine<State> stateMachine;

		// After emit, cooldown until next emit is possible (spaces emits)
		public float cooldownTime;

		// Max # of emitted objects alive at one time (prevents flooding)
		public int maxAlive;	

		protected HashSet<WeakReference<GameObject>> aliveEmits;
		protected Timer cooldownTimer;

		/*
		 * CAREFUL: this subclasses MonoBehaviour, constructor isn't called
		 */
		public Emitter(float cooldownTime, int maxAlive)
		{
			this.cooldownTime = cooldownTime;
			this.maxAlive = maxAlive;
		}

		protected virtual void EvtUpdate(TimeSlice time)
		{
			switch (stateMachine.State)
			{
				case State.Emit:
					break;
				case State.Cooldown:
					cooldownTimer.EvtUpdate(time);
					if (cooldownTimer.IsFinished)
					{
						stateMachine.State = State.Emit;
					}
					break;
			}
		}

		public GameObject Spawn()
		{
			GameObject spawn = NextSpawn();
			GameObject result = Instantiate(spawn, transform.position, Quaternion.identity);	// Clone it

			return result;
		}

		protected void RefreshAliveEmits()
		{
			var removeEmits = new List<WeakReference<GameObject>>();
			foreach (WeakReference<GameObject> r in aliveEmits)
			{
				// Either A) a game object is marked "dead" in script, 
				// or B) accessing its name produces an exception (means the object was destroyed)
				// or C) the WeakReference target is gone (Garbage Collector caught up).
				// For best performance, we have to check all 3.
				if (r.TryGetTarget(out GameObject go))
				{
					try
					{
						if (go.name == "dead")
						{
							removeEmits.Add(r);
						}
					}
					catch
					{
						removeEmits.Add(r);
					}
				}
				else
				{
					removeEmits.Add(r);
				}
			}

			foreach (WeakReference<GameObject> r in removeEmits)
			{
				aliveEmits.Remove(r);
			}
		}

		virtual protected bool CanEmit()
		{
			switch (stateMachine.State)
			{
				case State.Cooldown:
					return false;
			}

			RefreshAliveEmits();
			if (maxAlive > 0 && aliveEmits.Count >= maxAlive)
			{
				return false;
			}

			return true;
		}

		virtual public GameObject Emit()
		{
			if (!CanEmit())
			{
				return null;
			}

			var result = Spawn();
			if (null != result)
				aliveEmits.Add(new WeakReference<GameObject>(result));

			if (cooldownTime > 0)
			{
				stateMachine.State = State.Cooldown;
				cooldownTimer.duration = cooldownTime;
				cooldownTimer.Reset();
			}

			return result;
		}

		protected override void Start()
		{
			base.Start();

			// MonoBehavior subclasses require creating child objects in Start
			stateMachine = new GenericStateMachine<State>();
			aliveEmits = new HashSet<WeakReference<GameObject>>();
			cooldownTimer = new Timer();
		}

		protected virtual void Update()
		{
			EvtUpdate(new TimeSlice(Time.deltaTime));
		}
	}
}
