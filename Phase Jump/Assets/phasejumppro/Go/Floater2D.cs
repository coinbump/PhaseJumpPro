using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using PJ;

/*
 * RATING: 5 stars. Simple, common, effective.
 * CODE REVIEW: 12/18/18
 */
namespace PJ {
	/// <summary>
	/// Floats up through 3 states: float, hold, fade.
	/// When it's done fading, it's destroyed.
	/// 
	/// EXAMPLE: enemy is hit, show damage floating off and fade it out
	/// EXAMPLE: player matches something, gets a score, float that up
	/// </summary>
	public class Floater2D : Node2D
	{
		public enum State
		{
			Float,
			Hold,
			Fade
		}
		protected StateMachine<State> stateMachine;

		[System.Serializable]
		public class FloatInfo
		{
			public State state;
			public float duration;
			public float velocity;	// Always float up
		}
		public List<FloatInfo> floatInfos;

		// Internal use only.
		protected class Core : PJ.Core
		{
			private Floater2D owner;

			public Core(Floater2D owner)
			{
				this.owner = owner;
				SetStateMachine(owner.stateMachine);
			}

			protected override void EvtStateChanged(SomeStateMachine state)
			{
				base.EvtStateChanged(state);

				if (state == owner.stateMachine)
				{
					owner.EvtStateChanged();
				}
			}

			protected override void EvtStateFinished(SomeStateMachine state)
			{
				base.EvtStateFinished(state);

				if (state == owner.stateMachine)
				{
					owner.EvtStateFinished();
				}
			}
		}
		protected Core core;

		protected void EvtStateFinished() {
			State nextState = State.Hold;

			switch (stateMachine.State)
			{
				case State.Float:
					FloatInfo holdInfo = InfoForState(State.Hold);

					if (null == holdInfo || (null != holdInfo && Utils.CompareEqual(holdInfo.duration, 0)))
					{
						nextState = State.Fade;
					}
					break;
				case State.Hold:
					nextState = State.Fade;
					break;
				case State.Fade:
					Destroy(gameObject);
					return;
			}
		
			stateMachine.State = nextState;
		}

		protected void EvtStateChanged()
		{
			UpdateVelocity();

			FloatInfo floatInfo = InfoForState(stateMachine.State);
			if (null != floatInfo)
			{
				stateMachine.SetStateDuration(floatInfo.duration);

				if (Utils.CompareEqual(floatInfo.duration, 0))
				{
					EvtStateFinished();
				}
			}
			else
			{
				Debug.Log(string.Format("WARNING. Missing info for {0}", stateMachine.State));
			}
		}

		protected void UpdateVelocity() { 
			FloatInfo floatInfo = InfoForState(stateMachine.State);
			if (null != floatInfo)
			{
				velocity = Vector2.up * floatInfo.velocity;
			}
			else
			{
				Debug.Log(string.Format("WARNING. Missing info for {0}", stateMachine.State));
			}     
		}

		protected FloatInfo InfoForState(State state)
		{
			foreach (FloatInfo floatInfo in floatInfos)
			{
				if (floatInfo.state == state)
				{
					return floatInfo;
				}
			}
			return null;
		}

		protected override void Awake()
		{
			base.Awake();

			stateMachine = new StateMachine<State>();
			core = new Core(this);

			SetupDefaults();
		}

		protected void Reset()
		{
			floatInfos = new List<FloatInfo>();
			SetupDefaults();
		}

		protected void SetupDefaults() {
			isKinematic = true;

			if (floatInfos.Count == 0)
			{
				floatInfos.Add(new FloatInfo
				{
					state = State.Float,
					duration = 0.75f,
					velocity = 2.0f
				});
				floatInfos.Add(new FloatInfo
				{
					state = State.Hold,
					duration = 1.0f,
					velocity = 1.0f
				});
				floatInfos.Add(new FloatInfo
				{
					state = State.Fade,
					duration = 0.5f,
					velocity = 2.0f
				});
			}
		}

		protected override void OnValidate()
		{
			base.OnValidate();
		}

		protected override void Start()
		{
			base.Start();

			EvtStateChanged();	// Force setup of velocity and timers
		}

		protected override void Update()
		{
			base.Update();

			stateMachine.EvtUpdate(new TimeSlice(Time.deltaTime));

			switch (stateMachine.State)
			{
				case State.Fade:
					var interpolate = new InterpolateOutSquared();
					float progress = interpolate.Evaluate(stateMachine.Progress);
					float alpha = 1.0f - progress;
					var sr = GetComponent<SpriteRenderer>();
					var color = sr.color;
					color.a = alpha;
					sr.color = color;
					break;
			}
		}
	}
}


