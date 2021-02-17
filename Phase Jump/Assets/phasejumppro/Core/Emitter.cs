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
		StateMachine<State> stateMachine;

		// After emit, cooldown until next emit is possible (spaces emits)
		public float cooldownTime;

		// Max # of emitted objects alive at one time (prevents flooding)
		public int maxAlive;	

		protected WeakObjectSet aliveEmits;
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

		virtual protected bool CanEmit()
		{
			switch (stateMachine.State)
			{
				case State.Cooldown:
					return false;
			}

			aliveEmits.Refresh();
			if (maxAlive > 0 && aliveEmits.Count >= maxAlive)
			{
				return false;
			}

			return true;
		}

		public virtual GameObject Emit()
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
			stateMachine = new StateMachine<State>();
			aliveEmits = new WeakObjectSet();
			cooldownTimer = new Timer();
		}

		protected virtual void Update()
		{
			EvtUpdate(new TimeSlice(Time.deltaTime));
		}
	}
}
