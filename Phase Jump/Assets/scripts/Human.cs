using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class Human : PJ.Node2D
{

	public enum State
	{
		Invalid,
		WalkLeft,
		WalkRight,
		RunLeft,
		RunRight
	}

	Behavior behavior;

	class IsBoredBehavior : ConditionBehavior
	{
		public override bool Evaluate()
		{
			if (!owner.IsAlive) { return false; }
			var human = owner.Target as Human;

			// Going from walk to run near a wall causes issues
			Debug.Log(String.Format("Active Colliders: {0}", human.activeColliders.Count));
			return human.boredom >= 5.0f && human.activeColliders.Count == 0;
		}
	}

	class WalkBehavior : Behavior
	{
		protected override void _Run()
		{
			base._Run();

			if (!owner.IsAlive) { return; }
			var human = owner.Target as Human;

			if (human.IsMovingRight()) {
				human.core.state.State = Human.State.WalkRight;
			}
			else {
				human.core.state.State = Human.State.WalkLeft;
			}
			state.State = State.Success;
		}
	}

	class RunBehavior : TimerBehavior {
		public RunBehavior()
			:	base(2.0f) {

		}

		protected override void _Run() {
			base._Run();

			if (!owner.IsAlive) { return; }
			var human = owner.Target as Human;

			if (human.IsMovingRight())
			{
				human.core.state.State = Human.State.RunRight;
			}
			else
			{
				human.core.state.State = Human.State.RunLeft;
			}
		}
	}

	float boredom;

	class Core : PJ.Core
	{
		WeakReference owner;

		public GenericStateMachine<State> state = new GenericStateMachine<State>();

		public Core(WeakReference owner)
		{
			this.owner = owner;
			state.State = State.WalkLeft;
			SetStateMachine(state);
		}

		protected override void EvtStateChanged(AbstractStateMachine state)
		{
			base.EvtStateChanged(state);

			if (!owner.IsAlive)
			{
				return;
			}

			var human = owner.Target as Human;
			human.EvtStateChanged();
		}
	}
	Core core;
	Animator animator;

	void EvtStateChanged() {
		
		UpdateVelocity();

		GetComponent<SpriteRenderer>().flipX = IsMovingRight();

		if (IsRunning()) {
			boredom = 0;
			animator.SetFloat("speed", 4.0f);
		}
		else {
			animator.SetFloat("speed", 1.0f);
		}
	}

	bool IsRunning()
	{
		switch (core.state.State)
		{
			case State.RunLeft:
			case State.RunRight:
				return true;
		}
		return false;
	}

	bool IsMovingRight() {

		switch (core.state.State)
		{
			case State.WalkRight:
			case State.RunRight:
				return true;
		}
		return false;
	}

	void Start() {
		core = new Core(new WeakReference(this));
		animator = gameObject.GetComponent<Animator>();

		// Build the behavior tree (simple)
		behavior = new Behavior(new WeakReference(this));
		var isBoredBehavior = new IsBoredBehavior();
		behavior.AddChild(isBoredBehavior);
		var runBehavior = new RunBehavior();
		isBoredBehavior.AddChild(runBehavior);
		var walkBehavior = new WalkBehavior();
		behavior.AddChild(walkBehavior);

		UpdateVelocity();
	}

	HashSet<Collider2D> activeColliders = new HashSet<Collider2D>();
		
	private void OnCollisionEnter2D(Collision2D coll)
	{
		if (coll.gameObject.tag == "wall")
		{
			activeColliders.Add(coll.collider);	// Ignore the floor

			switch (core.state.State)
			{
				case State.WalkLeft:
					core.state.State = State.WalkRight;
					break;
				case State.WalkRight:
					core.state.State = State.WalkLeft;
					break;
				case State.RunLeft:
					core.state.State = State.RunRight;
					break;
				case State.RunRight:
					core.state.State = State.RunLeft;
					break;
			}
		}
	}

	private void OnCollisionExit2D(Collision2D coll)
	{
		activeColliders.Remove(coll.collider);
	}

	void UpdateVelocity() {
		var walkSpeed = IsRunning() ? 4.0f : 2.0f;

		switch (core.state.State) {
			case State.WalkLeft:
				velocity = new Vector2(-walkSpeed, 0);
				break;
			case State.WalkRight:
				velocity = new Vector2(walkSpeed, 0);
				break;
		}
	}
	
	void FixedUpdate() {

		var r = GetComponent<Rigidbody2D>();
		r.velocity = velocity;
	}

	void Update()
	{
		base.UpdateNode();

		behavior.EvtUpdate(new TimeSlice(Time.deltaTime));

		switch (core.state.State)
		{
			case State.WalkLeft:
			case State.WalkRight:
				boredom += Time.deltaTime;
				break;
		}

	}
}
