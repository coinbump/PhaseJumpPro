using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

/// <summary>
/// Has a basic behavior tree: walks for awhile, gets bored, does some running
/// </summary>
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

			return human.boredom >= 5.0f;
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

	class RunBehavior : TimedBehavior {
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

		public StateMachine<State> state = new StateMachine<State>();

		public Core(WeakReference owner)
		{
			this.owner = owner;
			state.State = State.WalkLeft;
			SetStateMachine(state);
		}

		protected override void EvtStateChanged(SomeStateMachine state)
		{
			base.EvtStateChanged(state);

			if (!owner.IsAlive) { return; }
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

	protected override void Start() {
		base.Start();

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
		if (IsMovingRight()) {
			velocity = new Vector2(walkSpeed, 0);
		}
		else {
			velocity = new Vector2(-walkSpeed, 0);
		}
	}
	
	protected override void FixedUpdate() {

		var r = GetComponent<Rigidbody2D>();
		r.velocity = velocity;
	}

	protected override void Update()
	{
		base.UpdateNode(UpdateType.Default);

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
