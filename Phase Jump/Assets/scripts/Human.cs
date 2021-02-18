using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

/// <summary>
/// Has a basic behavior tree: walks for awhile, gets bored, does some running
/// </summary>
public class Human : Node2D
{
	static class State
    {
		public const string Invalid = "invalid";
		public const string WalkLeft = "walk-left";
		public const string WalkRight = "walk-right";
		public const string RunLeft = "run-left";
        public const string RunRight = "run-right";
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
				human.state.State = Human.State.WalkRight;
			}
			else {
				human.state.State = Human.State.WalkLeft;
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
				human.state.State = Human.State.RunRight;
			}
			else
			{
				human.state.State = Human.State.RunLeft;
			}
		}
	}

	float boredom;

	new Core core;

    protected override void EvtStateChanged(SomeStateMachine state) {
		
		UpdateVelocity();

		GetComponent<SpriteRenderer>().flipX = IsMovingRight();
		var animator = GetComponent<Animator>();

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
		switch (state.State)
		{
			case State.RunLeft:
			case State.RunRight:
				return true;
		}
		return false;
	}

	bool IsMovingRight() {

		switch (state.State)
		{
			case State.WalkRight:
			case State.RunRight:
				return true;
		}
		return false;
	}

    protected override void Awake()
    {
        base.Awake();
    }

    protected override void Start() {
		//base.Start();
		Debug.Log("Log: Start");

		state.State = State.WalkLeft;

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

			switch (state.State)
			{
				case State.WalkLeft:
					state.State = State.WalkRight;
					break;
				case State.WalkRight:
					state.State = State.WalkLeft;
					break;
				case State.RunLeft:
					state.State = State.RunRight;
					break;
				case State.RunRight:
					state.State = State.RunLeft;
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

		switch (state.State)
		{
			case State.WalkLeft:
			case State.WalkRight:
				boredom += Time.deltaTime;
				break;
		}

	}
}
